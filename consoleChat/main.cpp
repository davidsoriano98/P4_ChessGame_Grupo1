#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include <string>
#include <thread>
#include <random>
#include "TCPSocketManager.h"
#include "TCPClient.h"
#include "TCPServer.h"
#include "ChessBoard.h"

const unsigned short PORT = 5000;
const sf::IpAddress IP = "127.0.0.1";
bool applicationRunning = true;

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

void OpenReceiveThread(TCPClient* _tcpClient)
{
	while (applicationRunning)
	{
		_tcpClient->Receive();
	}
}

void OpenListener(TCPServer* _tcpServer)
{
	_tcpServer->AddListener(PORT);

	while (applicationRunning)
	{
		_tcpServer->Listen(PORT, IP);
	}
}

void OpenBoardGame(TCPClient client)
{
	ChessBoard board;
	board.Run(client);
}

void Server()
{
	std::cout << "Server mode running" << std::endl;

	TCPServer tcpServer;

	sf::Packet infoPacket;
	std::string sendMessage, receiveMessage;
	int clientID = 1;

	// Logic for receiving
	std::thread tcpScoketListen(OpenListener, &tcpServer);
	tcpScoketListen.detach();

	std::thread getLines(GetLineFromCin, &sendMessage);
	getLines.detach();

	while (applicationRunning)
	{

	}

	tcpServer.Disconnect();
}

void Client()
{
	std::cout << "Client mode running" << std::endl;
	
	TCPClient tcpClient;
	TCPSocketManager tcpSocketManager;
	bool createdBoardThread = false;

	// client connect
	sf::Socket::Status status = tcpClient.Connect(PORT, IP);

	sf::Packet infoPacket;
	std::string sendMessage, receiveMessage;

	// Logic for receiving
	std::thread tcpSocketReceive(OpenReceiveThread, &tcpClient);
	tcpSocketReceive.detach();

	std::thread getLines(GetLineFromCin, &sendMessage);
	getLines.detach();

	if (tcpClient.GetID() == 0)
	{
		tcpClient.SendLogin();
	}

	while (applicationRunning)
	{
		if (status != sf::Socket::Done)
		{
			applicationRunning = false;
			break;
		}

		if (sendMessage.size() != 0)
		{
			// Send message
		}

		if (!createdBoardThread && tcpClient.GetHasRival())
		{
			std::thread boardGameThread(OpenBoardGame, tcpClient);
			boardGameThread.detach();

			createdBoardThread = true;
		}
	}

	sendMessage = "exit";
	tcpClient.SendMessage(infoPacket, &sendMessage);
	tcpClient.Disconnect();
}

void main()
{
	std::srand(time(0));

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