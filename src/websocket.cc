#include "websocket.h"
#include <iostream>

#include <cassert>

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
