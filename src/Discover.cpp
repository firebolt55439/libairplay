#include "Discover.h"
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

std::vector<AirplayDevice> discoveredHosts;

void resolveCallback(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *fullname, const char *hosttarget, uint16_t port, uint16_t, const unsigned char*, void*){
	// Handle result, if no error.
	if(errorCode == kDNSServiceErr_NoError){
		// Swap the port byte order from network (big-endian) to little-endian.
		port = __builtin_bswap16(port);
		
		// Get the IP address for the specified domain name.
		printf("Querying service host name...\n");
		struct hostent* host = gethostbyname(hosttarget);
		struct in_addr** addr_list = (struct in_addr**)host->h_addr_list;
		char* resolved_ip = inet_ntoa(*addr_list[0]);
		printf("Resolved service to %s:%d.\n", resolved_ip, port);
		
		// Save the host.
		discoveredHosts.push_back(AirplayDevice({(int)interfaceIndex, resolved_ip, port}));
	}
}

void browseCallback(DNSServiceRef sdRef, DNSServiceFlags flags, uint32_t interfaceIndex, DNSServiceErrorType errorCode, const char *serviceName, const char *regtype, const char *replyDomain, void *context){
	if(errorCode == kDNSServiceErr_NoError){
		printf("Discovered service '%s' registered as '%s' on domain '%s', resolving...\n", serviceName, regtype, replyDomain);
		
		// Resolve service, if possible.
		DNSServiceRef resolveRef = NULL;
		if(DNSServiceResolve(&resolveRef, flags, interfaceIndex, serviceName, regtype, replyDomain, &resolveCallback, NULL) == kDNSServiceErr_NoError){
			// Process the result.
			DNSServiceProcessResult(resolveRef);
		} else {
			std::cerr << "Could not resolve service.\n";
		}
		
		// Clean up.
		DNSServiceRefDeallocate(resolveRef);
	}
}

unsigned int AirplayBrowser::browseForDevices(){
	// Initialize an empty DNSServiceRef that will be filled if DNSServiceBrowse(...) succeeds.
	DNSServiceRef browseRef = NULL;
	
	// Browse for services.
	std::cout << "Searching...\n";
	if(DNSServiceBrowse(&browseRef, (DNSServiceFlags)0, kDNSServiceInterfaceIndexAny, AirplayRegtype, NULL, &browseCallback, NULL) == kDNSServiceErr_NoError){
		// Block until the mDNS daemon's response is received and process the result.
		if(DNSServiceProcessResult(browseRef) != kDNSServiceErr_NoError){
			std::cerr << "Error processing mDNS daemon response!\n";
			return -1;
		}
	} else {
		std::cerr << "An error occured while browsing for services.\n";
		return -1;
	}
	
	// Clean up.
	DNSServiceRefDeallocate(browseRef);
	discovered = discoveredHosts;
	
	return discovered.size();
}




