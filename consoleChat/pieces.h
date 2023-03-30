#pragma once
#include <iostream>
#include <vector>

class Piece
{
private:
	int arrayOfChess[64];

public:
	Piece(int Chess[]);

	bool IsTileOccupied(int tile);

	std::string PieceAlliance(int tile, std::string alliance);

};


class king
{
private:
	const int CANDIDATE_MOVE_COORDINATES[8] = { -9, -8, -7, -1, 1, 8, 7, 9 };
	int pieceTile;
	std::string alliance;
	int arrOfChess[64];

public:
	king(int pieceTile, std::string alliance, const int chess[]);

	bool isValidTileCoordinate(int currentCandidate);

	//-------------------------------EXCLUSIONS

	bool firstColum(int pieceTile, int offset);

	bool eighthColum(int pieceTile, int offset);

	//--------------------EXCLUSIONS

	std::vector<int> GetLegalMoves();
};


class knight
{
private:
	const int CANDIDATE_MOVE_COORDINATES[8] = { -17, -15, -10, -6, 6, 10, 15, 17 };
	int pieceTile;
	std::string alliance;
	int arrOfChess[64];

public:
	knight(int pieceTile, std::string alliance, int chess[]);

	bool isValidTileCoordinate(int currentCandidate);

	//-------------------------------EXCLUSIONS

	bool firstColum(int pieceTile, int offset);

	bool secondColum(int pieceTile, int offset);

	bool seventhColum(int pieceTile, int offset);

	bool eighthColum(int pieceTile, int offset);

	//--------------------EXCLUSIONS

	std::vector<int> GetLegalMoves();
};

class pawn
{
private:
	const int CANDIDATE_MOVE_COORDINATES[4] = { 7, 8, 9, 16 };
	int pieceTile;
	int arrOfChess[64];
	std::string alliance;
	signed int direction;

public:
	pawn(int Tile, std::string alliance, int chess[]);

	bool isValidTileCoordinate(int currentCandidate);

	//------------------------------------------------------------------EXCLUSIONS
	//---------------------------------COLUMN

	bool eighthColum(int tile);

	bool firstColum(int tile);

	bool isFirstMove(int tile);

	//-----------------CALCULATIONS

	std::vector<int> GetLegalMoves();

};

class Queen
{
private:
	const int CANDIDATE_MOVE_COORDINATES[8] = { -8, -1, 1, 8, -9, -7, 9, 7 };
	int pieceTile;
	std::string alliance;
	int arrOfChess[64]{};

public:
	Queen(const int Tile, std::string alliance, int chess[]);

	bool IsValidTileCoordinate(int currentCandidate);

	//---------------------------EXCLUSIONS

	bool FirstColum(int tile, int offset);

	bool EighthColum(int tile, int offset);

	//---------------------------CALCULATIONS

	std::vector<int> GetLegalMoves();

};


class Rook
{
private:
	const int CANDIDATE_MOVE_COORDINATES[4] = { -8, -1, 1, 8 };
	int pieceTile;
	std::string alliance;
	int arrOfChess[64];

public:
	Rook(const int Tile, std::string alliance, int chess[]);

	bool IsValidTileCoordinate(int currentCandidate);

	//---------------------------EXCLUSIONS

	bool FirstColum(int tile, int offset);

	bool EighthColum(int tile, int offset);

	//---------------------------CALCULATIONS

	std::vector<int> GetLegalMoves();

};

class Bishop
{
private:
	const int CANDIDATE_MOVE_COORDINATES[4] = { -9, -7, 9, 7 };
	int pieceTile;
	std::string alliance;
	int arrOfChess[64];

public:
	Bishop(int pieceTile, std::string alliance, int chess[]);

	bool IsValidTileCoordinate(int currentCandidate);

	//----------------------------Exclusions

	bool FirstColum(int pieceTile, int offset);

	bool EighthColum(int pieceTile, int offset);

	//----------------------------CALCULATIONS

	std::vector<int> GetLegalMoves();
};

class Identity {
public:
	bool ChessKing(int initialTile, int finalTile, std::string alliance, int* arrOfChess);
	bool ChessQueen(int initialTile, int finalTile, std::string alliance, int* arrOfChess);
	bool ChessPawn(int initialTile, int finalTile, std::string alliance, int* arrOfChess);
	bool ChessRook(int initialTile, int finalTile, std::string alliance, int* arrOfChess);
	bool ChessBishop(int initialTile, int finalTile, std::string alliance, int* arrOfChess);
	bool ChessKnight(int initialTile, int finalTile, std::string alliance, int* arrOfChess);
	std::string CheckAlliance(int piece);
	bool Identifier(int initialTile, int finalTile, int piece, int* arrOfChess);
public:
};
