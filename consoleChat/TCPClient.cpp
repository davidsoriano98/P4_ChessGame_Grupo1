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

void TCPClient::SendMove(int initialTile, int finalTile, int piece)
{
    sf::Packet packet;
    packet << TCPSocketManager::MAKE_MOVE << ID << initialTile << finalTile << piece;

    Send(packet);
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

        case TCPSocketManager::MOVE_RESPONSE:
            if (identification != ID)
                return;

            packet >> isValidMove;
            receivedValidation = true;
            std::cout << "MOVE_RESPONSE" << std::endl;
            break;

        case TCPSocketManager::UPDATE_GAME:
            if (identification != ID)
                return;

            std::cout << "UPDATE_GAME" << std::endl;
            ReceiveUpdateGame(packet);
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

void TCPClient::ReceiveUpdateGame(sf::Packet gamePack)
{
    int tempInitTile;
    int tempFinalTile;
    int tempPiece;

    gamePack >> tempInitTile >> tempFinalTile >> tempPiece;
    chessBoard->externalUpdateData.initialTile = tempInitTile;
    chessBoard->externalUpdateData.finalTile = tempFinalTile;
    chessBoard->externalUpdateData.piece = tempPiece;

    SetReceivedUpdate(true);
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

bool TCPClient::GetReceivedValidation()
{
    return receivedValidation;
}

bool TCPClient::GetIsValidMove()
{
    receivedValidation = false;
    return isValidMove;
}

bool TCPClient::GetReceivedUpdate()
{
    return receivedUpdate;
}

void TCPClient::SetReceivedUpdate(bool _receiveUpdate)
{
    receivedUpdate = _receiveUpdate;
}

void TCPClient::SetBoard(ChessBoard* _chessBoard)
{
    chessBoard = _chessBoard;
}
