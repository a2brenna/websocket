#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <string>

class Transport {

    public:
        virtual std::string read() = 0;
        virtual void write(const std::string &message) = 0;

};

class Connection_Failed {};

class Transport_Error {};

class TCP : public Transport {

    public:
        TCP(const std::string &host, const int &port);
        ~TCP();
        std::string read();
        void write(const std::string &message);

    private:
        int _fd;
        bool _open;

};

class TLS : public Transport {

    public:
        std::string read();
        void write(const std::string &message);

};

#endif