#include <sstream>
#include <iostream>

#include "common.hpp"
#include "airplay_device.hpp"

const std::string airplay_device::APPLE_DEVICE_ID = "0xdc2b61a0ce79";
const std::string airplay_device::APPLE_SESSION_ID = "1bd6ceeb-fffd-456c-a09c-996053a7a08c";

std::unique_ptr<safe_socket> airplay_device::connect_to_device(const address& addr) {
    auto sock = std::make_unique<safe_socket>(SocketType::TCP);
    sock->connect(addr);
    return sock;
}

std::string airplay_device::send_message(MessageType type) {
    std::string message = build_message(type, "");
    _socket->send(buffer(message.begin(), message.end()));

    buffer response = _socket->recv(1 * 1024);
    return std::string(response.data(), response.size());
}

std::string airplay_device::send_message(MessageType type, const std::string& args) {
    std::string message = build_message(type, args);
    _socket->send(buffer(message.begin(), message.end()));

    buffer response = _socket->recv(1 * 1024);
    return std::string(response.data(), response.size());
}

std::string airplay_device::send_message(MessageType type, const buffer& args) {
    std::string message = build_message(type, std::string(args.data(), args.size()));
    _socket->send(buffer(message.begin(), message.end()));

    buffer response = _socket->recv(1 * 1024);
    return std::string(response.data(), response.size());
}

std::string airplay_device::build_message(MessageType type, const std::string& args) {
    switch (type) {
        case MessageType::GetServices:
        {
            return "GET /server-info HTTP/1.1\nX-Apple-Device-ID: " + APPLE_DEVICE_ID + "\nContent-Length: 0\nUser-Agent: MediaControl/1.0\nX-Apple-Session-ID: " + APPLE_SESSION_ID + "\n\n";
        }
        case MessageType::PlayVideo:
        {
            std::string body = "Content-Location: " + std::string(args) + "\nStart-Position: 0.00\n\n";
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
        case MessageType::StopPlayback:
        {
            return "POST /stop HTTP/1.1\nUser-Agent: Media-Control/1.0\nContent-Length: 0\n\n";
        }
        case MessageType::GetSlideshowFeatures:
        {
            return "GET /slideshow-features HTTP/1.1\nAccept-Language: English\nX-Apple-Device-ID: " + APPLE_DEVICE_ID + "\nContent-Length: 0\nUser-Agent: MediaControl/1.0\nX-Apple-Session-ID: " + APPLE_SESSION_ID + "\n\n";
        }
        case MessageType::ShowPicture:
        {
            std::string body = args + "\n\n";
            std::stringstream fmt;
            fmt << "PUT /photo HTTP/1.1\nX-Apple-Asset-Key: F92F9B91-954E-4D63-BB9A-EEC771ADE6E8\n";
            fmt << "X-Apple-Transition: Dissolve\nContent-Length: " << body.length() << "\nUser-Agent: Media-Control/1.0\n";
            fmt << "X-Apple-Session-ID: " << APPLE_SESSION_ID << "\n\n";
            fmt << body;
            return fmt.str();
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
