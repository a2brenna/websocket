#include "transport.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sstream>

#include <cassert>

//TODO: test fragmented messages by setting READ_BUFF_SIZE v small

struct addrinfo *get_addrinfo(const std::string &host, const int &port){
    //TODO: why not std::to_string(port)?
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
}

int get_fd(const struct addrinfo *r){
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
}

TCP::TCP(const std::string &host, const int &port, const size_t &buff_size){

    _buff_size = buff_size;

    struct addrinfo *r = get_addrinfo(host, port);

    _fd = get_fd(r);

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
    return;
}

std::string TCP::read(){
    if(_open){
        std::string msg;

        while(true){

            const ssize_t bytes_read = [](const int &fd, std::string &msg, const size_t &buff_size){
                const size_t init_msg_size = msg.size();
                msg.resize(init_msg_size + buff_size);

                const ssize_t r = [](const int &fd, std::string &msg, const size_t &init_msg_size, const size_t &buff_size){
                    if(init_msg_size == 0){
                        return ::read(fd, &msg[0], buff_size);
                    }
                    else{
                        return ::recv(fd, &msg[0] + init_msg_size, buff_size, MSG_DONTWAIT);
                    }
                }(fd, msg, init_msg_size, buff_size);

                if(r >= 0){
                    msg.resize(init_msg_size + r);
                }

                return r;
            }(_fd, msg, _buff_size);

            if (bytes_read < 0){
                throw Transport_Error();
            }
            else if(bytes_read == 0){
                _open = false;
                break;
            }
            else if(bytes_read < _buff_size){
                break;
            }
            else if(bytes_read == _buff_size){
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

TLS::TLS(const std::string &host, const int &port, const size_t &buff_size){

    _buff_size = buff_size;

    const auto global_init = gnutls_global_init();
    if(global_init != GNUTLS_E_SUCCESS){
        throw Transport_Error();
    }

    const auto credential_allocation = gnutls_certificate_allocate_credentials(&_credentials);
    if(credential_allocation != GNUTLS_E_SUCCESS){
        throw Transport_Error();
    }

    /*
    const auto certificate_set = gnutls_certificate_set_x509_trust_file(_credentials, "/etc/ssl/certs/ca-certificates.crt", GNUTLS_X509_FMT_PEM);
    if(certificate_set != GNUTLS_E_SUCCESS){
        throw Transport_Error();
    }
    */

    const auto session_init = gnutls_init(&_tls_session, GNUTLS_CLIENT | GNUTLS_NO_SIGNAL);
    if(session_init != GNUTLS_E_SUCCESS){
        throw Transport_Error();
    }

    const auto server_name_set = gnutls_server_name_set(_tls_session, GNUTLS_NAME_DNS, host.c_str(), host.size());
    if(server_name_set != GNUTLS_E_SUCCESS){
        throw Transport_Error();
    }

    const auto priority_status = gnutls_set_default_priority(_tls_session);
    if(priority_status != GNUTLS_E_SUCCESS){
        throw Transport_Error();
    }

    const auto credentials_set = gnutls_credentials_set(_tls_session, GNUTLS_CRD_CERTIFICATE, _credentials);
    if(credentials_set != GNUTLS_E_SUCCESS){
        throw Transport_Error();
    }

    //gnutls_session_set_verify_cert(_tls_session, host.c_str(), 0);

    struct addrinfo *r = get_addrinfo(host, port);
    _fd = get_fd(r);
    freeaddrinfo(r);
    if(_fd < 0){
        throw Transport_Error();
    }

    gnutls_transport_set_int(_tls_session, _fd);

    const auto handshake_status = gnutls_handshake(_tls_session);
    if(handshake_status != GNUTLS_E_SUCCESS){
        throw Transport_Error();
    }

    _open = true;
}

TLS::~TLS(){
    close(_fd);
    gnutls_deinit(_tls_session);
    gnutls_global_deinit();
}

std::string TLS::read(){
    if(_open){
        std::string msg;

        do{

            const size_t init_msg_size = msg.size();
            msg.resize(init_msg_size + _buff_size);

            const auto bytes_recvd  = gnutls_record_recv(_tls_session, &msg[0] + init_msg_size, _buff_size);

            //Error
            if(bytes_recvd < 0){
                throw Transport_Error();
            }

            msg.resize(init_msg_size + bytes_recvd);

            if(bytes_recvd == 0){
                _open = false;
                break;
            }

        }while(gnutls_record_check_pending(_tls_session) != 0);

        return msg;
    }
    else{
        throw Transport_Error();
    }
}

void TLS::write(const std::string &message){
    if(_open){
        const auto send_result = gnutls_record_send(_tls_session, message.c_str(), message.size());
        if(send_result != (ssize_t)message.size()){
            throw Transport_Error();
        }
    }
    else{
        throw Transport_Error();
    }
    return;
}
