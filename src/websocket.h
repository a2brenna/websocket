#ifndef __WEBSOCKET_H__
#define __WEBSOCKET_H__

#include <string>
#include <vector>
#include <memory>

#include "transport.h"

class Bad_Address {};

class Address {

    public:
        Address(const std::string &uri);
        Address(const char *url);
        std::string host() const;
        std::string resource() const;
        int port() const;
        bool tls() const;

    private:
        std::string _host;
        std::string _resource;
        int _port;
        bool _tls;

};

class Client {

    public:
        Client(const Address &address);
        ~Client();
        std::vector<std::string> read();
        void write(const std::string &message);

    private:
        std::unique_ptr<Transport> _transport;
        std::string _buffer;
        std::vector<std::string> _process_buffer();

};

#endif
