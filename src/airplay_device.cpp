#include <iostream>

#include "common.hpp"
#include "airplay_device.hpp"

enum class MessageType : uint32_t {
    GetServices = 0,

    // Video
    PlayVideo,
    PauseVideo,
    ResumeVideo,
    StopPlaybak,

    // Photo
    GetSlideshowFeatures,
    ShowPicture,
    StopPicture,
};

const std::string airplay_device::APPLE_DEVICE_ID = "0xdc2b61a0ce79";
const std::string airplay_device::APPLE_SESSION_ID = "1bd6ceeb-fffd-456c-a09c-996053a7a08c";

std::unique_ptr<safe_socket> airplay_device::connect_to_device(const address& addr) {
    auto sock = std::make_unique<safe_socket>(SocketType::TCP);
    sock->connect(addr);
    return sock;
}

std::string airplay_device::get_services() {
    return send_message(MessageType::GetServices, "");
}

std::string airplay_device::get_slideshow_features() {
    return send_message(MessageType::GetSlideshowFeatures, "");
}

std::string airplay_device::send_message(MessageType type, std::string args) {
    std::string message = build_message(type, args);
    _socket->send(buffer(message.begin(), message.end()));

    buffer response = _socket->recv(1 * 1024);
    return std::string(response.data(), response.size());
}

std::string airplay_device::build_message(MessageType type, std::string args) {
    switch (type) {
        case MessageType::GetServices:
        {
            return "GET /server-info HTTP/1.1\nX-Apple-Device-ID: " + APPLE_DEVICE_ID + "\nContent-Length: 0\nUser-Agent: MediaControl/1.0\nX-Apple-Session-ID: " + APPLE_SESSION_ID + "\n\n";
        }
        case MessageType::PlayVideo:
        {
            std::string body = "Content-Location: " + args + "\nStart-Position: 0.00\n\n";
            std::string fmt = "POST /play HTTP/1.1\nContent-Length: %d\nUser-Agent: Media-Control/1.0\n\n%s";
            std::vector<char> buf(fmt.length() + body.length() + 20);
            sprintf(buf.data(), fmt.c_str(), body.length(), body.c_str());
            return std::string(buf.data());
        }
        case MessageType::PauseVideo:
        case MessageType::ResumeVideo:
        {
            std::string rate = (type == MessageType::PauseVideo ? "0.000000" : "1.000000");
            return "POST /rate?value=" + rate + " HTTP/1.1\nUser-Agent: Media-Control/1.0\nContent-Length: 0\n\n";
        }
        case MessageType::StopPlaybak:
        {
            return "POST /stop HTTP/1.1\nUser-Agent: Media-Control/1.0\nContent-Length: 0\n\n";
        }
        case MessageType::GetSlideshowFeatures:
        {
            return "GET /slideshow-features HTTP/1.1\nAccept-Language: English\nX-Apple-Device-ID: " + APPLE_DEVICE_ID + "\nContent-Length: 0\nUser-Agent: MediaControl/1.0\nX-Apple-Session-ID: " + APPLE_SESSION_ID + "\n\n";
        }
        case MessageType::ShowPicture:
        {
            // TODO: Receive the image somehow
        }
        case MessageType::StopPicture:
        {
            return "POST /stop HTTP/1.1\nContent-Length: 0\nUser-Agent: MediaControl/1.0\nX-Apple-Session-ID: " + APPLE_SESSION_ID + "\n\n";
        }
        default:
        {
            CHECK_AND_THROW(false, "Invalid message type");
        }
    }
}
