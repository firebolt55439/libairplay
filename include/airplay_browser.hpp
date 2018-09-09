#pragma once

#include <map>
#include <dns_sd.h>

#include "address.hpp"

class airplay_browser {
public:
    airplay_browser();

    static std::map<std::string, address> get_devices();

private:
    DNSServiceRef _dns_browse_ref;
    std::string _current_resolved_service_name;
    std::map<std::string, address> _devices;

    static const std::string AIRPLAY_REGTYPE;

    virtual ~airplay_browser();
    static void browse_callback(DNSServiceRef sdRef,
                                DNSServiceFlags flags,
                                uint32_t interfaceIndex,
                                DNSServiceErrorType errorCode,
                                const char *serviceName,
                                const char *regtype,
                                const char *replyDomain,
                                airplay_browser *context);
    static void resolve_callback(DNSServiceRef sdRef,
                                 DNSServiceFlags flags,
                                 uint32_t interfaceIndex,
                                 DNSServiceErrorType errorCode,
                                 const char *fullname,
                                 const char *hosttarget,
                                 uint16_t port,
                                 uint16_t,
                                 const unsigned char*,
                                 airplay_browser *context);
    void wait_for_devices();
};
