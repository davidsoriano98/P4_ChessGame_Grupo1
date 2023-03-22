#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include <string>
#include <thread>
#include "TCPSocketManager.h"
#include "TCPClient.h"
#include "TCPServer.h"
#include "ChessBoard.h"

const unsigned short PORT = 5000;
const sf::IpAddress IP = "127.0.0.1";
bool applicationRunning = true;

enum Mode
{
	SERVER,
	CLIENT,
	COUNT
};

std::string username;
bool hasLogedIn = false;

// Function to read from console (adapted for threads)
void GetLineFromCin(std::string* mssg) 
{
	while (applicationRunning) 
	{
		std::string line;
		std::getline(std::cin, line);
		mssg->assign(line);
	}
}

void OpenReceiveThread(TCPClient* _tcpClient, std::string* _mssg)
{
	while (applicationRunning)
	{
		_tcpClient->Receive(_mssg);
	}
}

void OpenListener(TCPSocketManager* _tcpSocketManager)
{
	_tcpSocketManager->AddListener(PORT);

	while (applicationRunning)
	{
		_tcpSocketManager->Listen(PORT, IP);
	}
}

bool SendLogic(TCPSocketManager* tcpSocketManager, Mode mode, sf::Packet mssgInfo, std::string* message)
{
	if (message->size() > 0)
	{
		if (*message == "exit")
		{
			// Desconection
			switch (mode)
			{
			case SERVER:
				tcpSocketManager->ServerSendAll("DISCONNECT");
				break;
			case CLIENT:
				std::cout << "CLIENT DISCONECT" << std::endl;
				mssgInfo << tcpSocketManager->DISCONNECT << username << *message;
				tcpSocketManager->ClientSend(mssgInfo);
				break;
			default:
				break;
			}

			applicationRunning = false;
			message->clear();
			return false;
		}
		else
		{
			switch (mode)
			{
			case SERVER:
				tcpSocketManager->ServerSendAll(*message);
				break;
			case CLIENT:
				if (!hasLogedIn)
				{
					username = *message;
					hasLogedIn = true;
					mssgInfo << tcpSocketManager->LOGIN << username << *message;
				}
				else
				{
					mssgInfo << tcpSocketManager->MESSAGE << username << *message;
				}
				tcpSocketManager->ClientSend(mssgInfo);
				break;
			default:
				break;
			}
			message->clear();
		}
	}

	return true;
}

void Server()
{
	std::cout << "Server mode running" << std::endl;

	TCPSocketManager tcpSocketManager;

	sf::Packet infoPacket;
	std::string sendMessage, receiveMessage;

	// Logic for receiving
	std::thread tcpScoketListen(OpenListener, &tcpSocketManager);
	tcpScoketListen.detach();

	std::thread getLines(GetLineFromCin, &sendMessage);
	getLines.detach();

	while (applicationRunning)
	{
		// Logic for sending
		if (SendLogic(&tcpSocketManager, Mode::SERVER, infoPacket, &sendMessage) != true)
		{
			break;
		}
	}

	tcpSocketManager.Disconnect();
}

void Client()
{
	std::cout << "Client mode running" << std::endl;
	
	TCPClient tcpClient;
	TCPSocketManager tcpSocketManager;

	// client connect
	sf::Socket::Status status = tcpClient.Connect(PORT, IP);

	sf::Packet infoPacket;
	std::string sendMessage, receiveMessage;

	// Logic for receiving
	std::thread tcpSocketReceive(OpenReceiveThread, &tcpClient, &receiveMessage);
	tcpSocketReceive.detach();

	std::thread getLines(GetLineFromCin, &sendMessage);
	getLines.detach();

	while (applicationRunning)
	{
		if (status != sf::Socket::Done)
		{
			applicationRunning = false;
			break;
		}

		// Logic for sending
		if (tcpClient.SendMessage(infoPacket, &sendMessage) != true)
		{
			break;
		}
	}

	sendMessage = "exit";
	tcpClient.SendMessage(infoPacket, &sendMessage);
	tcpClient.Disconnect();
}

void main()
{
	ChessBoard board;
	board.run();

	int server_mode;
	std::string mode_str;
	std::cout << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
	server_mode = std::stoi(mode_str);

	if (server_mode == 1) 
	{
		Server();
	}
	else if (server_mode == 2)
	{
		Client();
	}
}