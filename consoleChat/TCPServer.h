#pragma once
#include <SFML\Network.hpp>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <algorithm>
#include <random>
#include "TCPSocketManager.h"

class TCPServer : TCPSocketManager
{
private:
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::map<int, sf::TcpSocket*> users;

public:
    sf::Socket::Status Listen(unsigned short port, sf::IpAddress ip);
    void Send(sf::Packet sendPacket, int id);
    void SendAll(std::string message);
    void Receive(sf::Packet receivedPacket, int id);
    void ClientDisconected(int id);
    void Disconnect();
    void AddListener(unsigned short port);
};

