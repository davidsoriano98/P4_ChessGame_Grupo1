#include "TCPServer.h"

sf::Socket::Status TCPServer::Listen(unsigned short port, sf::IpAddress ip)
{
    // Make the selector wait for data on any socket
    if (selector.wait())
    {
        // Test the listener
        if (selector.isReady(listener))
        {
            // The listener is ready: there is a pending connection
            sf::TcpSocket* client = new sf::TcpSocket;
            if (listener.accept(*client) == sf::Socket::Done)
            {
                // Add the new client to the clients list
                users[++idValue] = client;

                // Add the new client to the selector so that we will
                // be notified when he sends something
                selector.add(*client);

                std::cout << "Added client" << std::endl;
            }
            else
            {
                // Error, we won't get a new connection, delete the socket
                delete client;
            }
        }
        else
        {
            // The listener socket is not ready, test all other sockets (the clients)
            for (auto it = users.begin(); it != users.end(); ++it)
            {
                sf::TcpSocket& client = *it->second;
                if (selector.isReady(client))
                {
                    // The client has sent some data, we can receive it
                    sf::Packet packet;
                    if (client.receive(packet) == sf::Socket::Done)
                    {
                        Receive(packet, it->first);
                    }
                }
            }
        }
    }

    return sf::Socket::Done;
}

bool TCPServer::Send(sf::Packet sendPacket, int id)
{
    if (users[id] != nullptr)
    {
        sf::Socket::Status status = users[id]->send(sendPacket);
        std::cout << "Message sent" << std::endl;

        if (status != sf::Socket::Status::Done)
        {
            // Error when sending data
            std::cout << "Error sending message" << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "Error user ID invalid" << std::endl;
        return false;
    }

    sendPacket.clear();
    return true;
}

void TCPServer::SendAll(std::string message)
{
    sf::Packet packet;
    packet << message;

    if (message == "exit")
    {
        SendAll("DISCONNECT");
        Disconnect();
    }

    // The listener socket is not ready, test all other sockets (the clients)
    for (auto it = users.begin(); it != users.end(); ++it)
    {
        sf::TcpSocket& client = *it->second;
        if (selector.isReady(client))
        {
            // The client has sent some data, we can receive it
            sf::Packet packet;
            if (client.receive(packet) == sf::Socket::Done)
            {
                Receive(packet, it->first);
            }
        }
    }

    packet.clear();
}

void TCPServer::Receive(sf::Packet receivedPacket, int id)
{
    int tempMode;
    int tempID;
    receivedPacket >> tempMode >> tempID;

    // Check if user exists
    if (users.find(tempID) == users.end() && tempMode != LOGIN)
        return;

    switch (tempMode)
    {
    case TCPSocketManager::LOGIN:
        ReceiveLogin();
        break;

    case TCPSocketManager::MESSAGE:
        ReceiveMessage(receivedPacket, id);
        break;

    case TCPSocketManager::MAKE_MOVE:
        ReceiveMakeMove(receivedPacket, id);

        break;

    case TCPSocketManager::DISCONNECT:
        ClientDisconected(id);
        break;
    default:
        break;
    }
}

void TCPServer::ClientDisconected(int id)
{
    users[id]->disconnect();
    users.erase(id);
    std::cout << "Client disconnected" << std::endl;
}

void TCPServer::Disconnect()
{
    listener.close();

    for (auto it = users.begin(); it != users.end(); ++it)
    {
        ClientDisconected(it->first);
    }
}

void TCPServer::AddListener(unsigned short port)
{
    listener.listen(port);
    selector.add(listener);
}

void TCPServer::NewWaitingUser(int newUserID)
{
    sf::Packet infoPacket;
    if (waitingUsersIDs.size() > 0)
    {
        bool IsStartingFirst = (0 + (rand() % (1 - 0 + 1)) == 1);
        ChessGame game;
        game.firstID = waitingUsersIDs.front();
        game.secondID = newUserID;
        chessGames.emplace_back(game);

        // Joint 2 clients
        playingUsersIDs.push_back(std::make_pair(waitingUsersIDs.front(), newUserID));
        waitingUsersIDs.pop_front();

        infoPacket.clear();
        infoPacket << TCPSocketManager::START_GAME << playingUsersIDs.back().first << IsStartingFirst;
        Send(infoPacket, playingUsersIDs.back().first);
        userBoard[playingUsersIDs.back().first] = &chessGames.back();

        infoPacket.clear(); 
        infoPacket << TCPSocketManager::START_GAME << playingUsersIDs.back().second << !IsStartingFirst;
        Send(infoPacket, playingUsersIDs.back().second);
        userBoard[playingUsersIDs.back().second] = &chessGames.back();
    }
    else
    {
        // Send to waiting
        waitingUsersIDs.push_back(newUserID);

        infoPacket.clear();
        infoPacket << TCPSocketManager::MESSAGE << newUserID << "Waiting for opponent";
        Send(infoPacket, newUserID);
    }
}

bool TCPServer::IsMoveValid(int initialTile, int finalTile, int piece, int* chessArray)
{
    return identity.Identifier(initialTile, finalTile, piece, chessArray);
}

void TCPServer::ReceiveLogin()
{
    sf::Packet sendPacket;
    sendPacket << (int)TCPSocketManager::LOGIN << idValue;

    Send(sendPacket, idValue);
    std::cout << "New user" << std::endl;
    NewWaitingUser(idValue);
}

void TCPServer::ReceiveMessage(sf::Packet packet, int id)
{
    std::string tempMssg;
    packet >> tempMssg;

    if (tempMssg.size() > 0)
    {
        if (tempMssg == "exit")
        {
            ClientDisconected(id);
            return;
        }
        std::cout << "Received message" << std::endl;
    }
}

void TCPServer::ReceiveMakeMove(sf::Packet packet, int id)
{
    sf::Packet outPacket;
    int tempInitialTile;
    int tempFinalTile;
    int tempPiece;
    bool tempIsValid;

    // Extract data
    packet >> tempInitialTile >> tempFinalTile >> tempPiece;

    // Check if move is valid
    tempIsValid = IsMoveValid(tempInitialTile, tempFinalTile, tempPiece, userBoard[id]->board);
    
    // Inform of valid move
    outPacket << TCPSocketManager::MOVE_RESPONSE << id << tempIsValid;
    Send(outPacket, id);

    // Update other user
    if (tempIsValid)
    {
        // Send packet to other player to update the board
        outPacket.clear();
        if (id == userBoard[id]->firstID)
        {
            outPacket << TCPSocketManager::UPDATE_GAME << userBoard[id]->secondID << tempInitialTile << tempFinalTile << tempPiece;
            Send(outPacket, userBoard[id]->secondID);
        }
        else
        {
            outPacket << TCPSocketManager::UPDATE_GAME << userBoard[id]->firstID << tempInitialTile << tempFinalTile << tempPiece;
            Send(outPacket, userBoard[id]->firstID);
        }
    }

    //std::cout << "Valid? " << tempIsValid << std::endl;
}

