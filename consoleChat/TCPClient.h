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
	int ID = 0;

public:
	void Send(sf::Packet infoPack);
	void Receive(std::string* mssg);
	bool SendMessage(sf::Packet mssgInfo, std::string* message);
	sf::Socket::Status Connect(unsigned short port, sf::IpAddress ip);
	void Disconnect();
};

