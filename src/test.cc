#include <iostream>
#include <cassert>
#include "websocket.h"
#include <chrono>
#include <thread>

const size_t TEST_BUFF_SIZE = 16;

int main(){
    std::cout << "Synchronous Websocket Test..." << std::endl;
    std::cout << "Test Complete." << std::endl;

    const Address address("ws://test.com:8080");
    assert(address.tls() == false);
    assert(address.host() == "test.com");
    assert(address.port() == 8080);
    const Address address2("wss://test.com:8080");
    assert(address2.tls() == true);
    assert(address2.host() == "test.com");
    assert(address2.port() == 8080);
    const Address address3("wss://test.com");
    assert(address3.tls() == true);
    assert(address3.host() == "test.com");
    assert(address3.port() == 443);
    const Address address4("ws://test.com");
    assert(address4.tls() == false);
    assert(address4.host() == "test.com");
    assert(address4.port() == 80);
    const Address address5("ws://test.com/foo");
    assert(address5.tls() == false);
    assert(address5.host() == "test.com");
    assert(address5.port() == 80);
    assert(address5.resource() == "/foo");


    Client c(std::string("wss://echo.websocket.org/"), TEST_BUFF_SIZE);

    for(size_t i = 0 ; i < 10; i++){
        c.write(std::to_string(i));
        for(const auto &m: c.read()){
            std::cout << m << std::endl;
        }
    }

    const std::string long_test = "7c c4 2f 04 5d ea 6d 93 c6 18 96 0f 50 9c c4 4e";

    assert(long_test.size() > TEST_BUFF_SIZE);

    c.write(long_test);

	for(const auto &m: c.read()){
		std::cout << m << std::endl;
	}

    return 0;
}
