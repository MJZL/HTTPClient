#include "stdafx.h"
#include "BasicHTTP.h"
#define DEFAULT_BUFLEN 512

BasicHTTP::BasicHTTP()
{
}

BasicHTTP::~BasicHTTP()
{
	Disconnect();
}

void BasicHTTP::Connect(const char * hostname, int port)
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		cout << "WSAStartup failed. The failure code is:" << iResult << endl;
		exit(1);
	}

	win_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (win_socket == INVALID_SOCKET)
	{
		cout << "Could not connect. The error code is:" << WSAGetLastError() << endl;
		WSACleanup();
		exit(1);
	}

	struct hostent *host;
	host = gethostbyname(hostname);

	//https://docs.microsoft.com/en-us/windows/desktop/winsock/sockaddr-2
	char* localIP = inet_ntoa(*(struct in_addr *)*host->h_addr_list);

	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(port);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = inet_addr(localIP);


	cout << "Connecting...\n";

	iResult = connect(win_socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr));
	if (iResult == SOCKET_ERROR)
	{
		cout << "Could not connect. The error code is:" << iResult << endl;
		Disconnect();
		exit(1);
	}
	cout << "Connected." << endl;
}

void BasicHTTP::Shutdown()
{
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(win_socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown failed:" << WSAGetLastError() << endl;
		Disconnect();
	}
	cout << "Socket shutdown successfully." << endl;
}

void BasicHTTP::Disconnect()
{
	closesocket(win_socket);
	WSACleanup();
}

std::string BasicHTTP::Get(const char * hostname, int port, const char * resource, const char * opt_urlencoded)
{
	Connect(hostname, port);

	std::string response;
	// Build request
	std::string req = "GET";
	req.append(" ");
	req.append(resource);
	req.append(" HTTP/1.1\r\n");

	req.append("Host: ");
	req.append(hostname);
	req.append(":");
	req.append(to_string(port));
	req.append("\r\n");
	req.append("Cache-Control: no-cache\r\n");
	req.append("Connection: close\r\n\r\n");

	cout << "=============================== GET"
		<< endl
		<< req
		<< endl
		<< "=============================== "
		<< endl;

	Send(req);

	Receive(response);
	Shutdown();
	Disconnect();
	return response;
}

std::string BasicHTTP::Post(const char * hostname, int port, const char * resource, const char * opt_urlencoded)
{
	Connect(hostname, port);

	std::string response;
	// Build request
	std::string req = "POST";
	req.append(" ");
	req.append(resource);
	req.append(" HTTP/1.1\r\n");

	req.append("Host: ");
	req.append(hostname);
	req.append(":");
	req.append(to_string(port));
	req.append("\r\n");


	req.append("Cache-Control: no-cache\r\n");
	req.append("Content-length: ");
	req.append(to_string(strlen(opt_urlencoded)));
	req.append("\r\n");
	req.append("Content-Type: application/x-www-form-urlencoded\r\n\r\n");

	// User is required to handle URI encoding for this value
	req.append(opt_urlencoded);

	cout << "=============================== POST"
		<< endl
		<< req
		<< endl
		<< "=============================== "
		<< endl;

	Send(req);
	Receive(response);
	Disconnect();
	return response;
}

void BasicHTTP::Send(std::string &req)
{
	int iResult = send(win_socket, req.c_str(), req.size(), 0);
	if (iResult == SOCKET_ERROR)
		cout << "send failed:" << WSAGetLastError() << endl;
	else
		cout << "Bytes Sent:" << iResult << endl;
}


void BasicHTTP::Receive(std::string & response)
{
	char buffer[DEFAULT_BUFLEN];
	int iResult;
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set); /* clear the set */
	FD_SET(win_socket, &set); /* add our file descriptor to the set */
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	do
	{
		iResult = select(win_socket + 1, &set, NULL, NULL, &timeout);
		if (iResult == SOCKET_ERROR)
		{
			// select error...
		}
		else if (iResult == 0)
		{
			// timeout, socket does not have anything to read
			break;
		}
		else
		{
			iResult = recv(win_socket, buffer, DEFAULT_BUFLEN, 0);

			if (iResult > 0)
				response.append(buffer, 0, iResult);
			else if (iResult == 0)
				cout << "Connection closed." << endl;
			else
				cout << "Receive failed:" << WSAGetLastError() << endl;
		}
		
	} while (iResult > 0);
}
