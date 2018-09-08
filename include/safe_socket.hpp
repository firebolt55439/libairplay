//
//  safe_socket.hpp (Partial module - only TCP client functions)
//
//  Created by Liran on 07/09/2018.
//  Copyright © 2018 LPWS. All rights reserved.
//

#pragma once

#include <vector>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>

#include "address.hpp"
#include "auto_close_fd.hpp"

enum class SocketType : int {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
};

using buffer = std::vector<char>;

class safe_socket;
using safe_socket_ptr = std::shared_ptr<safe_socket>;

class safe_socket final {
public:
    safe_socket(SocketType type) :
        _fd(create_socket(type))
    {};
    safe_socket(int fd) :
        _fd(fd)
    {};
    virtual ~safe_socket();

    void connect(const address& remote_address);
    void send(const buffer& data);
    buffer recv(size_t bytes_to_read);

private:
    const auto_close_fd _fd;

    static int create_socket(SocketType type);
};
