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
    struct ChessGame
    {
        int firstID;
        int secondID;
        int board[64] = { -1,-2,-3,-4,-5,-3,-2,-1,
                            -6,-6,-6,-6,-6,-6,-6,-6,
                            0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0,
                            6, 6, 6, 6, 6, 6, 6, 6,
                            1, 2, 3, 4, 5, 3, 2, 1 };
    };
    std::list<ChessGame> chessGames;

    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::map<int, sf::TcpSocket*> users;
    unsigned int idValue = 0;

    std::list<int> waitingUsersIDs;
    std::list<std::pair<int, int>> playingUsersIDs;
    std::map<int, ChessGame*> userBoard;

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
    void ReceiveMakeMove(sf::Packet packet, int id);
};