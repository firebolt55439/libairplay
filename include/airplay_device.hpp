#pragma once

#include <memory>

#include "address.hpp"
#include "safe_socket.hpp"

struct AirplayImage {
    uint32_t size;
    void* data;
};

enum class MessageType : uint32_t;

class airplay_device {
public:
    airplay_device(const address& airplay_address) :
        _socket(connect_to_device(airplay_address))
    {}
    ~airplay_device() = default;

    std::string get_services();
    std::string get_slideshow_features();

    // TODO: Export more functions

private:
    const std::unique_ptr<safe_socket> _socket;

    static const std::string APPLE_DEVICE_ID;
    static const std::string APPLE_SESSION_ID;

    static std::unique_ptr<safe_socket> connect_to_device(const address& addr);
    std::string send_message(MessageType type, std::string args);
    static std::string build_message(MessageType type, std::string args);
};
