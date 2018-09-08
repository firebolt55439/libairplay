//
//  safe_socket.cpp
//
//  Created by Liran on 07/09/2018.
//  Copyright © 2018 LPWS. All rights reserved.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "common.hpp"
#include "safe_socket.hpp"

safe_socket::~safe_socket() {
    try {
        ::shutdown(_fd.get_fd(), SHUT_RDWR);
    }
    catch (...) {
        PRINT_ANY_EXCEPTION();
    }
}

int safe_socket::create_socket(SocketType type) {
    return ::socket(AF_INET, static_cast<int>(type), 0);
}

void safe_socket::connect(const address& remote_address) {
    const sockaddr_in sock_addr = remote_address.to_sockaddr_in();
    CHECK_AND_THROW(-1 != ::connect(_fd.get_fd(),
                                    reinterpret_cast<const sockaddr*>(&sock_addr),
                                    sizeof(sock_addr)),
                    "connect failed");
}

void safe_socket::send(const buffer& data) {
    CHECK_AND_THROW(-1 != ::send(_fd.get_fd(),
                                 data.data(),
                                 data.size(),
                                 0),
                    "send failed")
}

buffer safe_socket::recv(size_t bytes_to_read) {
    buffer result(bytes_to_read);
    
    ssize_t bytes_read = ::recv(_fd.get_fd(), result.data(), result.size(), 0);
    CHECK_AND_THROW(-1 != bytes_read, "recv failed");
    
    result.resize(bytes_read);
    return result;
}
