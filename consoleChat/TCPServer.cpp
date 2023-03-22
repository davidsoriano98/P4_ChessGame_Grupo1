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
                users[users.size() + 1] = client;

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

void TCPServer::Send(sf::Packet sendPacket, int id)
{
    if (users[id] != nullptr)
    {
        sf::Socket::Status status = users[id]->send(sendPacket);
        std::cout << "Message sent" << std::endl;
        if (status != sf::Socket::Status::Done)
        {
            // Error when sending data
            std::cout << "Error sending message" << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "Error user ID invalid" << std::endl;
        return;
    }

    sendPacket.clear();
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

    //for each (sf::TcpSocket * targetSocket in sockets)
    //{
    //    sf::Socket::Status status = targetSocket->send(packet);
    //    std::cout << "Message sent: " << message << std::endl;
    //    if (status != sf::Socket::Status::Done)
    //    {
    //        // Error when sending data
    //        std::cout << "Error sending message" << std::endl;
    //        return;
    //    }
    //}

    packet.clear();
}

void TCPServer::Receive(sf::Packet receivedPacket, int id = 0)
{
    //auto findResult;
    int tempMode;
    int tempID;
    std::string tempMssg;
    receivedPacket >> tempMode >> tempID >> tempMssg;

    sf::Packet sendPacket;

    switch (tempMode)
    {
    case TCPSocketManager::LOGIN:
        sendPacket << LOGIN << id;
        Send(sendPacket, id);
        std::cout << "New user" << std::endl;
        break;
    case TCPSocketManager::MESSAGE:
        if (tempMssg.size() > 0)
        {
            if (tempMssg == "exit")
            {
                ClientDisconected(id);
                return;
            }
            std::cout << "Received message" << std::endl;

            //send move to other player
        }
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

