#ifndef ACTIONS_INC
#define ACTIONS_INC
#include "Discover.h"
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const std::string APPLE_DEVICE_ID = "0xdc2b61a0ce79";
const std::string APPLE_SESSION_ID = "1bd6ceeb-fffd-456c-a09c-996053a7a08c";

enum MessageType {
	MESSAGE_GET_SERVICES = 0,
	MESSAGE_PLAY_VIDEO,
	MESSAGE_PAUSE_VIDEO,
	MESSAGE_RESUME_VIDEO,
	MESSAGE_STOP_PLAYBACK,
	MESSAGE_SHOW_PICTURE,
	MESSAGE_STOP_PICTURE
};

struct AirplayImage {
	unsigned int size;
	void* data;
};

class AirplayConnection {
	private:
		// Host information.
		int interfaceInd;
		char* ipAddress;
		int port;
		
		// Class information.
		int sock;
	public:
		AirplayConnection(AirplayDevice d);
		~AirplayConnection(){ }
		
		bool openConnection(); // attempt to open a connection to the airplay device
		void sendMessage(MessageType type, void* args = NULL, bool debug = false); // send a message with the specified arguments
};

#endif