#pragma once

#include <string>
#include <iostream>
#include <winsock2.h>
#include <windows.h>

using namespace std;

class BasicHTTP
{
public:
	BasicHTTP();
	~BasicHTTP();

	std::string Get(
		const char* hostname,
		int port,
		const char* resource,
		const char* opt_urlencoded);

	void Send(std::string &req);

	std::string Post(
		const char* hostname,
		int port,
		const char* resource,
		const char* opt_urlencoded);

	void Receive(std::string & response);

	void  Connect(
		const char * hostname,
		int port);

	void Disconnect();

	void Shutdown();

private:


	SOCKET win_socket = INVALID_SOCKET;
};