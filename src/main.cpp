#include <string>
#include <vector>
#include <iostream>

#include "airplay_device.hpp"
#include "airplay_browser.hpp"

__attribute__((weak)) int main() {
    std::cout << "[+] Fetching AirPlay devices:" << std::endl;
    const auto devices = airplay_browser::get_devices();
    for (auto device : devices) {
        std::cout << "     > " << device.first << std::endl;
    }

    std::cout << "[+] Connecting to device:" << std::endl;
    airplay_device appletv(devices.at("Living Room Apple TV"));
    std::cout << appletv.send_message(MessageType::GetServices) << std::endl;

    return 0;
}
