#include <unistd.h>

#include "common.hpp"
#include "auto_close_fd.hpp"

auto_close_fd::auto_close_fd(int fd) :
    _fd(fd)
{
    CHECK_AND_THROW(INVALID_FD_VALUE != _fd, "Invalid fd value");
}

auto_close_fd::~auto_close_fd() {
    try {
        if (_fd != INVALID_FD_VALUE) {
            close(_fd);
        }
    }
    catch (...)
    {
        PRINT_ANY_EXCEPTION();
    }
}

int auto_close_fd::get_fd() const {
    return _fd;
}
