#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "address.hpp"

address address::make_from_network(const sockaddr_in& sock_addr) {
    return address(ntohl(sock_addr.sin_addr.s_addr),
                   ntohs(sock_addr.sin_port));
}

address address::make_from_host(const sockaddr_in& sock_addr) {
    return address(sock_addr.sin_addr.s_addr, sock_addr.sin_port);
}

uint32_t address::get_address() const {
    return _address;
}

uint16_t address::get_port() const {
    return _port;
}

std::string address::get_printable_address() const {
    in_addr addr;
    addr.s_addr = ntohl(_address);

    return std::string(inet_ntoa(addr));
}

uint32_t address::parse_address(std::string address) {
    return htonl(inet_addr(address.c_str()));
}

sockaddr_in address::to_sockaddr_in() const {
    sockaddr_in result = {0};
    result.sin_family = AF_INET;
    result.sin_addr.s_addr = htonl(_address);
    result.sin_port = htons(_port);
    return result;
}
