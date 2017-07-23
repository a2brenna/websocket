#include "websocket.h"
#include <iostream>

#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <cassert>

const ssize_t READ_BUFF_SIZE = 4096;

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

TCP::TCP(const std::string &host, const int &port){

    struct addrinfo *r = [](const std::string &host, const int &port){

        const std::string port_string = [](const int &port){
            std::stringstream s;
            s << port;
            return s.str();
        }(port);

        const struct addrinfo hints = [](){
            struct addrinfo hints;
            //TODO: remove this?
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            return hints;
        }();

        struct addrinfo *r;
        const int addrinfo_status = getaddrinfo(host.c_str(), port_string.c_str(), &hints, &r);
        if((addrinfo_status != 0) || (r == nullptr)){
            throw Transport_Error();
        }
        else{
            return r;
        }
    }(host, port);

    _fd = [](const struct addrinfo *r){
        const int fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd < 0){
            throw Transport_Error();
        }

        const bool connected = [](const int &fd, const struct addrinfo *r){
            for(auto s = r; s != nullptr; s = s->ai_next){
                const int b = connect(fd, s->ai_addr, s->ai_addrlen);
                if (b == 0) {
                    return true;
                }
                else{
                    continue;
                }
            }

            return false;
        }(fd, r);

        if(connected){
            return fd;
        }
        else{
            close(fd);
            throw Connection_Failed();
        }
    }(r);

    _open = true;

    freeaddrinfo(r);
}

TCP::~TCP(){
    close(_fd);
}

void TCP::write(const std::string &message){
    if(_open){
        const auto s = ::write(_fd, message.c_str(), message.size());
        if(s != (ssize_t)message.size()){
            throw Transport_Error();
        }
    }
    else{
        throw Transport_Error();
    }
}

std::string TCP::read(){
    if(_open){
        std::string msg;

        while(true){

            const ssize_t bytes_read = [](const int &fd, std::string &msg){
                const size_t init_msg_size = msg.size();
                msg.resize(init_msg_size + READ_BUFF_SIZE);

                const ssize_t r = [](const int &fd, std::string &msg, const size_t &init_msg_size){
                    if(init_msg_size == 0){
                        return ::read(fd, &msg[0] + init_msg_size, READ_BUFF_SIZE);
                    }
                    else{
                        return ::recv(fd, &msg[0] + init_msg_size, READ_BUFF_SIZE, MSG_DONTWAIT);
                    }
                }(fd, msg, init_msg_size);

                if(r >= 0){
                    msg.resize(init_msg_size + r);
                }

                return r;
            }(_fd, msg);

            if (bytes_read < 0){
                throw Transport_Error();
            }
            else if(bytes_read == 0){
                _open = false;
                break;
            }
            else if(bytes_read < READ_BUFF_SIZE){
                break;
            }
            else if(bytes_read == READ_BUFF_SIZE){
                continue;
            }
            else{
                throw Transport_Error();
            }

        }

        return msg;
    }
    else{
        throw Transport_Error();
    }
}
