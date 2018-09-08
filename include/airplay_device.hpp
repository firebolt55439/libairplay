#pragma once

#include <memory>

#include "address.hpp"
#include "safe_socket.hpp"

enum class MessageType : uint32_t {
    GetServices = 0,
    PlayVideo,
    PauseVideo,
    ResumeVideo,
    StopPlayback,
    GetSlideshowFeatures,
    ShowPicture,
    StopPicture,
};

class airplay_device {
public:
    airplay_device(const address& airplay_address) :
        _socket(connect_to_device(airplay_address))
    {}
    ~airplay_device() = default;

    std::string send_message(MessageType type);
    std::string send_message(MessageType type, const std::string& args);
    std::string send_message(MessageType type, const buffer& args);

private:
    const std::unique_ptr<safe_socket> _socket;

    static const std::string APPLE_DEVICE_ID;
    static const std::string APPLE_SESSION_ID;

    static std::unique_ptr<safe_socket> connect_to_device(const address& addr);
    static std::string build_message(MessageType type, const std::string& args);
};
