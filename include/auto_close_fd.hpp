#pragma once

static const int INVALID_FD_VALUE = -1;

class auto_close_fd final {
public:
    explicit auto_close_fd(int fd);
    virtual ~auto_close_fd();

    int get_fd() const;

private:
    const int _fd;
};
