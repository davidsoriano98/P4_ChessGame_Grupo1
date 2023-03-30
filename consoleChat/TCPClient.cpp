#include "TCPClient.h"


void TCPClient::Send(sf::Packet infoPack)
{
    sf::Socket::Status status = serverSocket->send(infoPack);
    if (status != sf::Socket::Status::Done)
    {
        // Error when sending data
        std::cout << "Error sending message" << std::endl;
        return;
    }

    infoPack.clear();
}

void TCPClient::SendLogin()
{
    sf::Packet infoPack;
    infoPack << LOGIN << ID;

    Send(infoPack);
}

void TCPClient::Receive()
{
    sf::Packet packet;
    sf::Socket::Status status = serverSocket->receive(packet);

    if (status != sf::Socket::Status::Done)
    {
        std::cout << "Error receiving message" << std::endl;
        return;
    }
    else
    {
        int tempMode;
        int identification;
        packet >> tempMode >> identification;

        std::string tempString;

        switch (tempMode)
        {
        case TCPSocketManager::LOGIN:
            ID = identification;
            std::cout << "Received ID: " << ID << std::endl;
            break;

        case TCPSocketManager::MESSAGE:
            if (identification != ID)
                return;

            // Handle mssg
            packet >> tempString;
            std::cout << tempString << std::endl;

            break;

        case TCPSocketManager::START_GAME:
            if (identification != ID)
                return;

            hasRival = true;
            packet >> isMyTurn;

            break;

        case TCPSocketManager::DISCONNECT:
            if (identification != ID)
                return;
            // Manages the desconection
            Disconnect();
            break;

        default:
            break;
        }

        //std::cout << "Received message" << std::endl;
    }
}

bool TCPClient::SendMessage(sf::Packet mssgInfo, std::string* message)
{
	if (message->size() > 0)
	{
		if (*message == "exit")
		{
			// Desconection
			std::cout << "CLIENT DISCONECT" << std::endl;
			mssgInfo << DISCONNECT << ID << *message;
			Send(mssgInfo);

			message->clear();
			return false;
		}
		else
		{
            if (ID == 0)
            {
                mssgInfo << LOGIN << ID << *message;
            }
            else
            {
                mssgInfo << MESSAGE << ID << *message;
            }
            Send(mssgInfo);
			message->clear();
		}
	}

	return true;
}

sf::Socket::Status TCPClient::Connect(unsigned short port, sf::IpAddress ip)
{
    serverSocket = new sf::TcpSocket;

    sf::Socket::Status status = serverSocket->connect(ip, port, sf::seconds(5.f));
    if (status != sf::Socket::Done)
    {
        //No se ha podido conectar
        std::cout << "Error al conectarse con el servidor" << std::endl;
    }

    return status;
}

void TCPClient::Disconnect()
{
    serverSocket->disconnect();
}

int TCPClient::GetID()
{
    return ID;
}

bool TCPClient::GetHasRival()
{
    return hasRival;
}

bool TCPClient::GetIsMyTurn()
{
    return isMyTurn;
}
