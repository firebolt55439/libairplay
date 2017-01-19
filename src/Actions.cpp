#include "Discover.h"
#include "Actions.h"
#include <sstream>

AirplayConnection::AirplayConnection(AirplayDevice device){
	interfaceInd = device.interfaceInd;
	ipAddress = device.ipAddress;
	port = device.port;
}

bool AirplayConnection::openConnection(){
	// Create the socket.
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		std::cerr << "Error creating the socket.\n";
		return false;
	}
	
	// Connect to the host.
	struct sockaddr_in serv_addr;
	struct hostent* server = gethostbyname(ipAddress);
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	if(connect(sock, (const struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		std::cerr << "Error connecting to host.\n";
		return false;
	}
	
	// Signal success.
	return true;
}

std::string getMessage(MessageType type, void* args){
	if(type == MESSAGE_GET_SERVICES){
		return "GET /server-info HTTP/1.1\nX-Apple-Device-ID: " + APPLE_DEVICE_ID + "\nContent-Length: 0\nUser-Agent: MediaControl/1.0\nX-Apple-Session-ID: " + APPLE_SESSION_ID + "\n\n";
	} else if(type == MESSAGE_PLAY_VIDEO){
		std::string body = "Content-Location: " + std::string((char*)args) + "\nStart-Position: 0.00\n\n";
		std::string fmt = "POST /play HTTP/1.1\nContent-Length: %d\nUser-Agent: Media-Control/1.0\n\n%s";
		char* buf = new char[fmt.length() + body.length() + 20];
		sprintf(buf, fmt.c_str(), body.length(), body.c_str());
		std::string ret(buf);
		delete[] buf;
		return ret;
	} else if(type == MESSAGE_PAUSE_VIDEO || type == MESSAGE_RESUME_VIDEO){
		std::string rate = (type == MESSAGE_PAUSE_VIDEO ? "0.000000" : "1.000000");
		return "POST /rate?value=" + rate + " HTTP/1.1\nUser-Agent: Media-Control/1.0\nContent-Length: 0\n\n";
	} else if(type == MESSAGE_STOP_PLAYBACK){
		return "POST /stop HTTP/1.1\nUser-Agent: Media-Control/1.0\nContent-Length: 0\n\n";
	} else if(type == MESSAGE_SHOW_PICTURE){
		AirplayImage* img = (AirplayImage*) args;
		std::string body((const char*)img->data, img->size);
		body += "\n\n";
		std::stringstream fmt;
		fmt << "PUT /photo HTTP/1.1\nX-Apple-Asset-Key: F92F9B91-954E-4D63-BB9A-EEC771ADE6E8\n";
		fmt << "X-Apple-Transition: Dissolve\nContent-Length: " << body.length() << "\nUser-Agent: Media-Control/1.0\n";
		fmt << "X-Apple-Session-ID: " << APPLE_SESSION_ID << "\n\n";
		fmt << body;
		return fmt.str();
	} else if(type == MESSAGE_STOP_PICTURE){
		return "POST /stop HTTP/1.1\nContent-Length: 0\nUser-Agent: MediaControl/1.0\nX-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c\n\n";
	} else {
		std::cerr << "Warning: Invalid message type received: " << type << std::endl;
	}
	return "";
}

void AirplayConnection::sendMessage(MessageType type, void* args, bool debug){
	// Generate the message.
	std::string message = getMessage(type, args);
	
	// Send it.
	ssize_t sent = 0, total = (ssize_t)message.length();
	while(sent < total){
		sent += ::send(sock, message.substr(sent).c_str(), (size_t)(total - sent), 0);
	}
	if(debug) printf("Sent %zd/%zd bytes.\n", sent, total);
	
	// Wait for an ACK, if debug is true.
	if(debug){
		char buf[1025];
		ssize_t got = ::recv(sock, buf, 1024, 0);
		printf("%d bytes: |%s|.\n", (int)got, buf);
	}
}





















