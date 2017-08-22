#include "websocket.h"
#include <iostream>
#include <map>
#include <arpa/inet.h>

#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sodium.h>

#include "encode.h"

#include <cassert>

Address::Address(const char *uri) : Address(std::string(uri)) {}

Address::Address(const std::string &uri){

    size_t i = 0;

    std::pair<size_t, bool> tls = [](const std::string &uri, size_t i){
        //TODO: learn how to do string comparisons reasonably, idiot
        if(std::string(uri, i, 6) == "wss://"){
            return std::pair<size_t, bool>(6, true);
        }
        else if(std::string(uri, i, 5) == "ws://"){
            return std::pair<size_t, bool>(5, false);
        }
        else{
            throw Bad_Address();
        }
    }(uri, i);

    i = tls.first;
    _tls = tls.second;


    std::pair<size_t, std::string> host = [](const std::string &uri, size_t i){
        const size_t host_start_pos = i;
        for( ; i < uri.size(); i++){
            if( (uri[i] == ':') || (uri[i] == '/') ){
                break;
            }
        }
        return std::pair<size_t, std::string>(i, std::string(uri, host_start_pos, (i - host_start_pos)));
    }(uri, i);

    if (host.second == ""){
        throw Bad_Address();
    }
    else{
        i = host.first;
        _host = host.second;
    }

    std::pair<size_t, int> port = [](const std::string &uri, size_t i, const bool &tls){
        if( (i == uri.size()) || (uri[i] == '/') ){
            if(tls){
                return std::pair<size_t, int>(i, 443);
            }
            else{
                return std::pair<size_t, int>(i, 80);
            }
        }
        else if (uri[i] == ':'){
            const std::string tail(uri, i + 1, uri.size() - (i + 1));
            size_t port_len;
            const int port = std::stoi(tail, &port_len, 10);
            return std::pair<size_t, int>(i + 1 + port_len, port);
        }
        else{
            throw Bad_Address();
        }
    }(uri, i, _tls);

    i = port.first;
    _port = port.second;

    std::pair<size_t, std::string> resource = [](const std::string &uri, size_t i){
        if(i == uri.size()){
            return std::pair<size_t, std::string>(i, "/");
        }
        else if (uri[i] == '/'){
            return std::pair<size_t, std::string>(i, std::string(uri, i));
        }
        else{
            throw Bad_Address();
        }
    }(uri, i);

    i = resource.first;
    _resource = resource.second;
}

std::string Address::host() const{
    return _host;
}

std::string Address::resource() const{
    return _resource;
}

int Address::port() const{
    return _port;
}

bool Address::tls() const{
    return _tls;
}

enum OPCODE {
	CONTINUATION = 0x00,
	TEXT_FRAME = 0x01,
	BINARY_FRAME = 0x02,
	CLOSE = 0x08,
	PING = 0x09,
	PONG = 0x0a
};

class E_MALFORMED_HEADER {};

class Header {

    public:

        Header(){

        };

        Header(const std::string &data){
            if(data.size() < 2){
                throw E_MALFORMED_HEADER();
            }

            fin = (data[0] & 0x80) == 0x80;
            opcode = (OPCODE)(data[0] & 0x0f);
            masked = (data[1] & 0x80) == 0x80;
            const int n = (data[1] & 0x7f);
            const uint64_t header_len = 2 + (n == 126? 2 : 0) + (n == 127? 8 : 0) + (masked? 4 : 0);

            if(data.size() < header_len){
                throw E_MALFORMED_HEADER();
            }

            const std::pair<uint64_t, int> size_info = [](const std::string &data, const int &n){
                if(n < 126){
                    return std::pair<uint64_t, int>(n, 2);
                }
                else if(n == 126){
                    uint64_t payload_len = 0;
                    payload_len |= ((uint64_t) data[2]) << 8;
                    payload_len |= ((uint64_t) data[3]);
                    return std::pair<uint64_t, int>(payload_len, 4);
                }
                else if(n == 127){
                    uint64_t payload_len = 0;
                    payload_len |= ((uint64_t) data[2]) << 56;
                    payload_len |= ((uint64_t) data[3]) << 48;
                    payload_len |= ((uint64_t) data[4]) << 40;
                    payload_len |= ((uint64_t) data[5]) << 32;
                    payload_len |= ((uint64_t) data[6]) << 24;
                    payload_len |= ((uint64_t) data[7]) << 16;
                    payload_len |= ((uint64_t) data[8]) << 8;
                    payload_len |= ((uint64_t) data[9]);
                    return std::pair<uint64_t, int>(payload_len, 10);
                }
                else{
                    assert(false);
                }
            }(data, n);

            payload_len = size_info.first;
            const auto mask_offset = size_info.second;

			if (masked) {
                mask[0] = ((uint8_t) data[mask_offset+0]) << 0;
                mask[1] = ((uint8_t) data[mask_offset+1]) << 0;
                mask[2] = ((uint8_t) data[mask_offset+2]) << 0;
                mask[3] = ((uint8_t) data[mask_offset+3]);
            }
            else {
                mask[0] = 0;
                mask[1] = 0;
                mask[2] = 0;
                mask[3] = 0;
			}

        };

        void set_masked(){
            masked = true;
            assert(sizeof(mask) == 4);
            randombytes_buf(mask, sizeof(mask));
            return;
        }
        void set_unmasked(){
            masked = false;
            mask[0] = 0;
            mask[1] = 0;
            mask[2] = 0;
            mask[3] = 0;
            return;
        }

        uint64_t header_len() const{
            uint64_t header_len = 0;
            if(payload_len <= 125){
                header_len = 2;
            }
            else if(payload_len < 65536){
                header_len = 4;
            }
            else{
                header_len = 10;
            }

            if(masked){
                header_len += 4;
            }

            return header_len;
        };

        std::string str() const{
            std::string sh;
            sh.resize(header_len());
            //TODO: memset?
            for(size_t i = 0; i < sh.size(); i++){
                sh[i] = 0;
            }

            //fin
            sh[0] = sh[0] | 0x80;

            //opcode
            sh[0] = sh[0] | opcode;

            //masked
            sh[1] = sh[1] | 0x80;

            //size
            int mask_offset = 0;
            if(payload_len < 126){
                const uint8_t n = payload_len;
                sh[1] = sh[1] | n;
                mask_offset = 2;
            }
            else if(payload_len < 65536){
                const uint8_t n = 126;
                sh[1] = sh[1] | n;

                const uint16_t net_size = htons(payload_len);
                ::memcpy(&(sh[2]), &net_size, sizeof(uint16_t));

                mask_offset = 4;
            }
            else{
                assert(false);
                const uint8_t n = 127;
                sh[1] = sh[1] | n;
                mask_offset = 10;
            }

            //mask
            for(size_t i = 0; i < 4; i++){
                sh[mask_offset + i] = mask[i];
            }

            return sh;
        };

        bool fin;
        bool rsv1;
        bool rsv2;
        bool rsv3;
        OPCODE opcode;
        bool masked;
        uint64_t payload_len;
		uint8_t mask[4];

};

Client::Client(const Address &address) : Client(address, 4096) {}

Client::Client(const Address &address, const size_t &buff_size){

    assert(buff_size > 0);

    if(address.tls()){
        _transport = std::unique_ptr<Transport>(new TLS(address.host(), address.port(), buff_size));
    }
    else{
        _transport = std::unique_ptr<Transport>(new TCP(address.host(), address.port(), buff_size));
    }

    const std::string opening = [](const Address &address){
        const std::string encoded_nonce = [](){
            unsigned char nonce[16];
            randombytes_buf(nonce, 16);
            return base64_encode(nonce, 16);
        }();

        std::string opening = "GET " + address.resource() + " HTTP/1.1\r\n";
        opening.append("Upgrade: websocket\r\n");
        opening.append("Connection: Upgrade\r\n");
        opening.append("Host: " + address.host() + "\r\n");
        opening.append("Origin: http://" + address.host() + "\r\n");
        opening.append("Sec-WebSocket-Key: " + encoded_nonce + "\r\n");
        opening.append("Sec-WebSocket-Version: 13\r\n");
        opening.append("\r\n");

        return opening;
    }(address);
    _transport->write(opening);

    const std::pair<std::vector<std::string>, std::string> response = [](const std::unique_ptr<Transport> &transport){
        const std::string r = transport->read();

        std::vector<std::string> lines;
        size_t line_start = 0;
        size_t i = 0;
        while(i < (r.size() - 3)){
            if( (r[i] == '\r') && (r[i+1] == '\n') && (r[i+2] == '\r') && (r[i+3] == '\n')){
                const std::string line(r, line_start, (i - line_start));
                lines.push_back(line);
                i += 4;
                line_start = i;
                break;
            }
            else if( (r[i] == '\r') && (r[i+1] == '\n') ){
                const std::string line(r, line_start, (i - line_start));
                lines.push_back(line);
                i += 2;
                line_start = i;
            }
            else{
                i += 1;
            }
        }

        const std::string remainder(r, i, r.size() - i);

        return std::pair<std::vector<std::string>, std::string>(lines,remainder);
    }(_transport);

    //assert(response.first.front() == "HTTP/1.1 101 Web Socket Protocol Handshake");

    const std::map<std::string, std::string> headers = [](const std::vector<std::string> &response_http){
        std::map<std::string, std::string> headers;

        auto i = response_http.begin();
        i++;

        for( ; i != response_http.end(); i++){
            const auto line = *i;

            size_t i = 0;
            while(i < line.size() - 3){
                if( (line[i] == ':') && (line[i+1] == ' ') ){
                    const std::string key(line, 0, i);
                    const std::string value(line, i + 2, line.size() - (i + 2));
                    headers[key] = value;
                    break;
                }
                else{
                    i += 1;
                }
            }
        }

        return headers;
    }(response.first);

    const std::string correct_accept = "";
    assert(true);
    //assert(correct_accept == headers.at("Sec-WebSocket-Accept"));

    _buffer = response.second;
}

Client::~Client(){

    //close

}

std::vector<std::string> Client::_process_buffer(){
    std::vector<std::string> messages;

    while(!_buffer.empty()){
        const Header h(_buffer);

        const auto frame_size = h.payload_len + h.header_len();

        if(h.opcode == PING){
            assert(false);
        }
        else if(h.opcode == PONG){
            assert(false);
        }
        else if(h.opcode == CLOSE){
            assert(!h.masked);
            if(h.payload_len > 0){
                const std::string payload(&_buffer[h.header_len()], h.payload_len);
                _buffer.erase(0, frame_size);

                assert(payload.size() >= 2);

                const uint16_t close_status = [](const std::string &payload){
                    uint16_t close_status;
                    ::memcpy(&close_status, &payload[0], 2);
                    return ntohs(close_status);
                }(payload);
            }
        }
        else{
            if(frame_size <= _buffer.size()){
                assert(!h.masked);
                messages.push_back(std::string(&_buffer[h.header_len()], h.payload_len));
                _buffer.erase(0, frame_size);
            }
            else{
                break;
            }
        }
    }

    return messages;
}

std::vector<std::string> Client::read(){
    std::vector<std::string> messages = _process_buffer();

    while(messages.empty()){
        _buffer.append(_transport->read());
        messages = _process_buffer();
    }

    return messages;
}

void Client::write(const std::string &message){
    (void)message;

    //Generate frame header
    Header h;
    h.fin = true;
    h.rsv1 = false;
    h.rsv2 = false;
    h.rsv3 = false;
    h.opcode = TEXT_FRAME;
    h.set_masked();
    h.payload_len = message.size();

    const std::string masked_payload = [](const std::string &message, const uint8_t mask[4]){
        std::string masked_payload;
        masked_payload.resize(message.size());

        for(size_t i = 0; i < message.size(); i++){
            masked_payload[i] = message[i] ^ mask[i % 4];
        }

        return masked_payload;
    }(message, h.mask);

    const std::string frame_header = h.str();

    std::cout << base16_encode(frame_header) << std::endl;

    _transport->write(frame_header + masked_payload);
}
