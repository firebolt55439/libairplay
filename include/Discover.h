#ifndef DISCOVER_INC
#define DISCOVER_INC
#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <stdint.h>
#include <dns_sd.h>

struct AirplayDevice {
	int interfaceInd;
	char* ipAddress;
	int port;
};

class AirplayBrowser {
	private:
		const char* AirplayRegtype = "_airplay._tcp";
		std::vector<AirplayDevice> discovered;
	public:
		AirplayBrowser(){ }
		~AirplayBrowser(){ }
		
		std::vector<AirplayDevice>& getDiscovered(){ return discovered; }
		unsigned int browseForDevices(); // returns number of devices found
};

#endif