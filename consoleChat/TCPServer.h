#pragma once
#include <SFML\Network.hpp>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <algorithm>
#include <random>
#include "TCPSocketManager.h"
#include "ChessBoard.h"
#include "pieces.h"

class TCPServer : TCPSocketManager
{
private:
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::map<int, sf::TcpSocket*> users;

    std::list<int> waitingUsersIDs;
    std::list<std::pair<int, int>> playingUsersIDs;

    unsigned int idValue = 0;

    ChessBoard chessBoard;
    Identity identity;

public:
    sf::Socket::Status Listen(unsigned short port, sf::IpAddress ip);
    bool Send(sf::Packet sendPacket, int id);
    void SendAll(std::string message);
    void Receive(sf::Packet receivedPacket, int id);
    void ClientDisconected(int id);
    void Disconnect();
    void AddListener(unsigned short port);
    void NewWaitingUser(int newUserID);
    bool IsMoveValid(int initialTile, int finalTile, int piece, int* chessArray);

    void ReceiveLogin();
    void ReceiveMessage(sf::Packet packet, int id);
    void ReceiveMakeMove(sf::Packet packet);
};