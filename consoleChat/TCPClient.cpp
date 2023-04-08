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

void TCPClient::SendContinuePlaying()
{
    sf::Packet packet;
    packet << TCPSocketManager::CONTINUE << ID;
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

            receivedGameClose = false;
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

        case TCPSocketManager::GAME_CLOSE:
            if (identification != ID)
                return;

            receivedGameClose = true; 

            // Reset values
            ResetValues();

            // Ask if they want to play again
            std::cout << "Want to play again? (Y/N)" << std::endl;
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

void TCPClient::SendWindowClosed(bool gameEnded)
{
    sf::Packet pack;

    pack << TCPSocketManager::GAME_CLOSE << ID << gameEnded;
    Send(pack);
}

void TCPClient::ResetValues()
{
    hasRival = false;
    isMyTurn = false;
    receivedValidation = false;
    isValidMove = false;
    receivedUpdate = false;
}

sf::Socket::Status TCPClient::Connect(unsigned short port, sf::IpAddress ip)
{
    serverSocket = new sf::TcpSocket;

    sf::Socket::Status status = serverSocket->connect(ip, port, sf::seconds(5.f));
    if (status != sf::Socket::Done)
    {
        std::cout << "Error connecting to the server" << std::endl;
    }

    return status;
}

void TCPClient::Disconnect()
{
    sf::Packet packet;
    packet << TCPSocketManager::DISCONNECT << ID;
    Send(packet);

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

bool TCPClient::GetReceivedGameClose()
{
    return receivedGameClose;
}

void TCPClient::SetReceivedUpdate(bool _receiveUpdate)
{
    receivedUpdate = _receiveUpdate;
}

void TCPClient::SetBoard(ChessBoard* _chessBoard)
{
    chessBoard = _chessBoard;
}

void TCPClient::SetReceivedGameClose(bool _receivedGameClose)
{
    receivedGameClose = _receivedGameClose;
}
