#pragma once
#include <SFML\Network.hpp>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <algorithm>
#include "TCPSocketManager.h"
#include "ChessBoard.h"

class TCPClient : TCPSocketManager
{
private:
	sf::TcpSocket* serverSocket;
	unsigned int ID = 0;

	bool hasRival = false;
	bool isMyTurn = false;

	bool receivedValidation = false;
	bool isValidMove = false;
	bool receivedUpdate = false;
	bool receivedGameClose = false;

	ChessBoard* chessBoard;

public:
	bool createdBoardThread = false;

	void Send(sf::Packet infoPack);
	void SendLogin();
	void SendMove(int initialTile, int finalTile, int piece);
	void SendContinuePlaying();
	void Receive();
	void ReceiveUpdateGame(sf::Packet gamePack);
	void SendWindowClosed(bool gameFinished);
	void ResetValues();
	
	sf::Socket::Status Connect(unsigned short port, sf::IpAddress ip);
	void Disconnect();

	int GetID();
	bool GetHasRival();
	bool GetIsMyTurn();
	bool GetReceivedValidation();
	bool GetIsValidMove();
	bool GetReceivedUpdate();
	bool GetReceivedGameClose();

	void SetReceivedUpdate(bool _receiveUpdate);
	void SetBoard(ChessBoard* _chessBoard);
	void SetReceivedGameClose(bool _receivedGameClose);
};

