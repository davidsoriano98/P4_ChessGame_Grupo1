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

void TCPClient::Receive(std::string* mssg)
{
    sf::Packet packet;

    sf::Socket::Status status = serverSocket->receive(packet);
    if (status != sf::Socket::Status::Done)
    {
        std::cout << "Error receiving message" << std::endl;
        return;
    }

    std::string masaje;
    packet >> masaje;

    // Se procesaelmensaje
    if (masaje.size() > 0)
    {
        if (masaje == "DISCONNECT")
        {
            // Manages the desconection
            Disconnect();
        }
        std::cout << "Received message: " << masaje << std::endl;
        mssg->assign(masaje);
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
