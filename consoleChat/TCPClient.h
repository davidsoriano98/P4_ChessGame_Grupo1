#pragma once
#include <SFML\Network.hpp>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <algorithm>
#include "TCPSocketManager.h"

class TCPClient : TCPSocketManager
{
private:
	sf::TcpSocket* serverSocket;
	unsigned int ID = 0;

public:
	void Send(sf::Packet infoPack);
	void SendLogin();
	void Receive();
	bool SendMessage(sf::Packet mssgInfo, std::string* message);
	sf::Socket::Status Connect(unsigned short port, sf::IpAddress ip);
	void Disconnect();
	int GetID();
};

