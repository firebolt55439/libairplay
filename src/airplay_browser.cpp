#include <iostream>
#include <exception>
#include <netdb.h>
#include <dns_sd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "common.hpp"
#include "airplay_browser.hpp"

const std::string airplay_browser::AIRPLAY_REGTYPE = "_airplay._tcp";

airplay_browser::airplay_browser() {
    CHECK_AND_THROW(kDNSServiceErr_NoError == DNSServiceBrowse(&_dns_browse_ref,
                                                               static_cast<DNSServiceFlags>(0),
                                                               kDNSServiceInterfaceIndexAny,
                                                               AIRPLAY_REGTYPE.c_str(),
                                                               nullptr,
                                                               reinterpret_cast<DNSServiceBrowseReply>(&browse_callback),
                                                               this),
                    "DNSServiceBrowse failed");
}

airplay_browser::~airplay_browser() {
    try {
        DNSServiceRefDeallocate(_dns_browse_ref);
    }
    catch (...) {
        PRINT_ANY_EXCEPTION();
    }
}

std::map<std::string, address> airplay_browser::get_devices() {
    airplay_browser browser;
    browser.wait_for_devices();
    return browser._devices;
}

void airplay_browser::browse_callback(DNSServiceRef sdRef,
                                      DNSServiceFlags flags,
                                      uint32_t interfaceIndex,
                                      DNSServiceErrorType errorCode,
                                      const char *serviceName,
                                      const char *regtype,
                                      const char *replyDomain,
                                      airplay_browser *context){
    CHECK_AND_THROW(kDNSServiceErr_NoError == errorCode, "browse_callback failed");

    context->_current_resolved_service_name = std::string(serviceName);

    DNSServiceRef dns_resolve_ref = nullptr;
    if(kDNSServiceErr_NoError != DNSServiceResolve(&dns_resolve_ref,
                                                   flags,
                                                   interfaceIndex,
                                                   serviceName,
                                                   regtype,
                                                   replyDomain,
                                                   reinterpret_cast<DNSServiceResolveReply>(&resolve_callback),
                                                   context)){
        std::cerr << "Could not resolve service" << std::endl;
        return;
    }

    DNSServiceProcessResult(dns_resolve_ref);
    DNSServiceRefDeallocate(dns_resolve_ref);
}

void airplay_browser::resolve_callback(DNSServiceRef sdRef,
                                       DNSServiceFlags flags,
                                       uint32_t interfaceIndex,
                                       DNSServiceErrorType errorCode,
                                       const char *fullname,
                                       const char *hosttarget,
                                       uint16_t port,
                                       uint16_t,
                                       const unsigned char*,
                                       airplay_browser *context){
    CHECK_AND_THROW(kDNSServiceErr_NoError == errorCode, "resolve_callback failed");

    const struct hostent* host = gethostbyname(hosttarget);
    const auto addr_list = reinterpret_cast<struct in_addr**>(host->h_addr_list);

    const std::pair<std::string, address> device_info(context->_current_resolved_service_name,
                                                      address(ntohl(static_cast<uint32_t>((addr_list[0])->s_addr)),
                                                              ntohs(port)));
    context->_devices.insert(device_info);

    context->_current_resolved_service_name = "";
}

void airplay_browser::wait_for_devices() {
    CHECK_AND_THROW(kDNSServiceErr_NoError == DNSServiceProcessResult(_dns_browse_ref),
                    "Error processing mDNS daemon response")
}
