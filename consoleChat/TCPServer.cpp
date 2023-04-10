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
            mtx.lock();
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
            mtx.unlock();
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


void TCPServer::SendDisconnect(int id)
{
    sf::Packet packet;
    packet << TCPSocketManager::DISCONNECT << id;
    Send(packet, id);
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

    case TCPSocketManager::MAKE_MOVE:
        ReceiveMakeMove(receivedPacket, id);
        break;

    case TCPSocketManager::GAME_CLOSE:
        ReceiveGameClose(receivedPacket, id);
        break;

    case TCPSocketManager::CONTINUE:
        NewWaitingUser(id);
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
    mtx.lock();
    users[id]->disconnect();
    users.erase(id);
    mtx.unlock();

    std::cout << "Client disconnected" << std::endl;
}

void TCPServer::Disconnect()
{
    std::list<int> tempIDs;
    mtx.lock();
    for (auto it = users.begin(); it != users.end(); ++it)
    {
        tempIDs.push_back(it->first);
    }
    mtx.unlock();

    int size = tempIDs.size();
    for (int i = 0; i < size; i++)
    {
        SendDisconnect(tempIDs.front());

        mtx.lock();
        users[tempIDs.front()]->disconnect();
        users.erase(tempIDs.front());
        mtx.unlock();

        tempIDs.pop_front();
    }

    listener.close();
    isServerRunning = false;
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


        infoPacket.clear();
        infoPacket << TCPSocketManager::START_GAME << waitingUsersIDs.front() << IsStartingFirst;
        Send(infoPacket, waitingUsersIDs.front());
        userBoard[waitingUsersIDs.front()] = &chessGames.back();

        infoPacket.clear();
        infoPacket << TCPSocketManager::START_GAME << newUserID << !IsStartingFirst;
        Send(infoPacket, newUserID);
        userBoard[newUserID] = &chessGames.back();

        waitingUsersIDs.pop_front();
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
        // Update server board
        userBoard[id]->board[tempInitialTile] = 0;
        userBoard[id]->board[tempFinalTile] = tempPiece;

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
}

void TCPServer::ReceiveGameClose(sf::Packet packet, int id)
{
    bool didGameEnd;
    sf::Packet outPacket;

    packet >> didGameEnd;

    // clean all user data related to game
    int otherPlayerId;
    if (id == userBoard[id]->firstID)
    {
        otherPlayerId = userBoard[id]->secondID;
    }
    else
    {
        otherPlayerId = userBoard[id]->firstID;
    }
    
    userBoard.erase(id);
    userBoard.erase(otherPlayerId);

    // Inform the players who won/lost if they finished the game (aka killed other player's king)
    if (didGameEnd)
    {
        outPacket << TCPSocketManager::MESSAGE << id << "YOU WIN!";
        Send(outPacket, id);

        outPacket.clear();
        outPacket << TCPSocketManager::MESSAGE << otherPlayerId << "YOU LOSE";
        Send(outPacket, otherPlayerId);
    }

    // inform other player of game close & ask both if they wanna play again
    outPacket.clear();
    outPacket << TCPSocketManager::GAME_CLOSE << id;
    Send(outPacket, id);

    outPacket.clear();
    outPacket << TCPSocketManager::GAME_CLOSE << otherPlayerId;
    Send(outPacket, otherPlayerId);
}

