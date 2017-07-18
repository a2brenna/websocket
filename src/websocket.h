#ifndef __WEBSOCKET_H__
#define __WEBSOCKET_H__

#include <string>
#include <memory>

class Transport {

    public:
        virtual std::string read() = 0;
        virtual void write(const std::string &message) = 0;

};

class TCP : public Transport {

    public:
        std::string read();
        void write(const std::string &message);

};

class TLS : public Transport {

    public:
        std::string read();
        void write(const std::string &message);

};

class Bad_Address {};

class Address {

    public:
        Address(const std::string &uri);
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
        std::string read();
        void write(const std::string &message);

    private:
        std::unique_ptr<Transport> _transport;

};

#endif
