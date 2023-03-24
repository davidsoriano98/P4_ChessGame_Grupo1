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

    unsigned int idValue = 0;

public:
    sf::Socket::Status Listen(unsigned short port, sf::IpAddress ip);
    bool Send(sf::Packet sendPacket, int id);
    void SendAll(std::string message);
    void Receive(sf::Packet receivedPacket, int id);
    void ClientDisconected(int id);
    void Disconnect();
    void AddListener(unsigned short port);
};

