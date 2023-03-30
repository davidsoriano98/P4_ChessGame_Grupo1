#pragma once
#include <iostream>
#include <vector>
#include "pieces.h"


Piece::Piece(int* Chess) {

	for (unsigned i = 0; i < 64; ++i) {
		arrayOfChess[i] = Chess[i];
	}
}

bool Piece::IsTileOccupied(int tile) {
	if (arrayOfChess[tile] == 0)
		return false;
	else
		return true;
}

std::string Piece::PieceAlliance(int tile, std::string alliance) {
	if (alliance == "white")
	{
		if (arrayOfChess[tile] < 0)
		{
			return "black";
		}
		else
		{
			return "white";
		}
	}

	else if (alliance == "black")
	{
		if (arrayOfChess[tile] > 0)
		{
			return "white";
		}
		else
			return "black";
	}
}



king::king(int pieceTile, std::string alliance, const int chess[]) {
    for (int i = 0; i < 64; ++i) {
        arrOfChess[i] = chess[i];
    }
    this->pieceTile = pieceTile;
    this->alliance = alliance;
}

bool king::isValidTileCoordinate(int currentCandidate) {
    return currentCandidate >= 0 && currentCandidate < 64;
}

bool king::firstColum(int pieceTile, int offset) {
    int firstColumArr[] = { 0,8,16,24,32,40,43,56 };
    bool temp = false;

    for (int i : firstColumArr)
    {
        if (pieceTile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;

    }

    return temp && (offset == -9 || offset == -1 || offset == 7);
}

bool king::eighthColum(int pieceTile, int offset) {
    int eighthColumArr[] = { 7,15,23,31,39,47,55,63 };
    bool temp = false;

    for (int i : eighthColumArr)
    {
        if (pieceTile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;
    }

    return temp && (offset == -7 || offset == 1 || offset == 9);
}

std::vector<int> king::GetLegalMoves() {

    Piece destinationTile(arrOfChess);
    int possibleDestinationTile = this->pieceTile;
    std::vector<int> legalMoves;
    //     int=new x;

    for (int offset : CANDIDATE_MOVE_COORDINATES)
    {

        if (firstColum(this->pieceTile, offset) || eighthColum(this->pieceTile, offset))
        {
            continue;
        }
        possibleDestinationTile = this->pieceTile + offset;
        if (isValidTileCoordinate(possibleDestinationTile))
        {
            if (destinationTile.IsTileOccupied(possibleDestinationTile)) {
                if (this->alliance != destinationTile.PieceAlliance(possibleDestinationTile, alliance)) {
                    legalMoves.push_back(possibleDestinationTile);
                }
            }
            else {
                legalMoves.push_back(possibleDestinationTile);
            }

        }

    }
    return legalMoves;
}


knight::knight(int pieceTile, std::string alliance, int chess[]) {
    for (int i = 0; i < 64; ++i) {
        arrOfChess[i] = chess[i];
    }
    this->pieceTile = pieceTile;
    this->alliance = alliance;
}

bool knight::isValidTileCoordinate(int currentCandidate) {
    return currentCandidate >= 0 && currentCandidate < 64;
}

bool knight::firstColum(int pieceTile, int offset) {
    int firstColumArr[] = { 0,8,16,24,32,40,43,56 };
    bool temp = false;

    for (int i : firstColumArr)
    {
        if (pieceTile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;

    }

    return temp && ((offset == -17) || (offset == -10) || (offset == 6) || (offset == 15));
}

bool knight::secondColum(int pieceTile, int offset) {
    int secondColumArr[] = { 1,9,17,25,33,41,49,55 };
    bool temp = false;

    for (int i : secondColumArr)
    {
        if (pieceTile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;

    }

    return temp && ((offset == -10) || (offset == 6));
}

bool knight::seventhColum(int pieceTile, int offset) {
    int seventhColumArr[] = { 6,14,22,30,38,46,54,62 };
    bool temp = false;

    for (int i : seventhColumArr)
    {
        if (pieceTile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;

    }

    return temp && ((offset == -6) || (offset == 10));
}

bool knight::eighthColum(int pieceTile, int offset) {
    int eighthColumArr[] = { 7,15,23,31,39,47,55,63 };
    bool temp = false;

    for (int i : eighthColumArr)
    {
        if (pieceTile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;
    }

    return temp && ((offset == -15) || (offset == -6) || (offset == 10) || (offset == 17));
}

std::vector<int> knight::GetLegalMoves() {

    Piece destinationTile(arrOfChess);
    int possibleDestinationTile;
    std::vector<int> legalMoves;

    for (int offset : CANDIDATE_MOVE_COORDINATES)
    {

        if (firstColum(this->pieceTile, offset) || secondColum(this->pieceTile, offset) ||
            seventhColum(this->pieceTile, offset) || eighthColum(this->pieceTile, offset))
        {
            continue;
        }
        possibleDestinationTile = this->pieceTile + offset;
        if (isValidTileCoordinate(possibleDestinationTile))
        {
            if (destinationTile.IsTileOccupied(possibleDestinationTile)) {
                if (this->alliance != destinationTile.PieceAlliance(possibleDestinationTile, this->alliance)) {
                    legalMoves.push_back(possibleDestinationTile);
                }
            }
            else {
                legalMoves.push_back(possibleDestinationTile);
            }

        }

    }
    return legalMoves;
}


pawn::pawn(int Tile, std::string alliance, int* chess) {
    for (int i = 0; i < 64; ++i) {
        arrOfChess[i] = chess[i];
    }

    this->pieceTile = Tile;
    this->alliance = alliance;

    if (this->alliance == "white")
    {
        direction = -1;
    }
    else if (this->alliance == "black")
    {
        direction = 1;
    }
}

bool pawn::isValidTileCoordinate(int currentCandidate) {
    return currentCandidate >= 0 && currentCandidate < 64;
}

bool pawn::eighthColum(int tile) {
    int eighthColumArr[] = { 7,15,23,31,39,47,55,63 };
    bool temp = false;

    for (int i : eighthColumArr)
    {
        if (tile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;
    }

    return temp;
}

bool pawn::firstColum(int tile) {
    int firstColumArr[] = { 0,8,16,24,32,40,43,56 };
    bool temp = false;

    for (int i : firstColumArr)
    {
        if (tile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;
    }

    return temp;
}

bool pawn::isFirstMove(int tile) {
    int secondRowArr[] = { 8,9,10,11,12,13,14,15 };
    int seventhRowArr[] = { 48,49,50,51,52,53,54,55 };

    if (alliance == "black")
    {
        for (int i : secondRowArr) {
            if (tile == i)
                return true;
        }
    }
    else if (alliance == "white")
    {
        for (int i : seventhRowArr) {
            if (tile == i)
                return true;
        }
    }

    return false;
}

std::vector<int> pawn::GetLegalMoves() {
    Piece destinationTile(arrOfChess);
    int possibleDestinationTile;
    std::vector<int> legalMoves;

    for (int offset : CANDIDATE_MOVE_COORDINATES)
    {
        possibleDestinationTile = this->pieceTile + (direction * offset);

        if (!isValidTileCoordinate(possibleDestinationTile))
        {
            continue;
        }

        if (offset == 8 && !destinationTile.IsTileOccupied(possibleDestinationTile))
        {
            legalMoves.push_back(possibleDestinationTile);
        }

        //TWO STEPS
        if (offset == 16 && isFirstMove(this->pieceTile))
        {
            int behindCandidate = this->pieceTile + (this->direction * 8);

            if (!destinationTile.IsTileOccupied(behindCandidate) && !destinationTile.IsTileOccupied(possibleDestinationTile))
            {
                legalMoves.push_back(possibleDestinationTile);
            }
        }


        if (offset == 7 && !((eighthColum(this->pieceTile) && this->alliance == "white") || (firstColum(this->pieceTile) && this->alliance == "black")))
        {
            if (destinationTile.IsTileOccupied(possibleDestinationTile))
            {
                if (this->alliance != destinationTile.PieceAlliance(possibleDestinationTile, alliance))
                {
                    legalMoves.push_back(possibleDestinationTile);
                }
            }
        }
        if (offset == 9 && !((firstColum(this->pieceTile) && this->alliance == "white") || (eighthColum(this->pieceTile) && this->alliance == "black")))
        {
            if (destinationTile.IsTileOccupied(possibleDestinationTile))
            {
                if (this->alliance != destinationTile.PieceAlliance(possibleDestinationTile, alliance))
                {
                    legalMoves.push_back(possibleDestinationTile);
                }
            }
        }

    }

    return legalMoves;
}


Queen::Queen(const int Tile, std::string alliance, int chess[]) {
    for (int i = 0; i < 64; ++i) {
        arrOfChess[i] = chess[i];
    }
    this->pieceTile = Tile;
    this->alliance = std::move(alliance);
}

bool Queen::IsValidTileCoordinate(int currentCandidate) {
    return currentCandidate >= 0 && currentCandidate < 64;
}

bool Queen::FirstColum(int tile, int offset) {
    int firstColumArr[] = { 0,8,16,24,32,40,43,56 };
    bool temp;

    for (int i : firstColumArr)
    {
        if (tile == i)
        {
            temp = true;
            break;
        }
        else {
            temp = false;
        }
    }

    return temp && (offset == -1 || offset == 7 || offset == -9);
}

bool Queen::EighthColum(int tile, int offset) {
    int eighthColumArr[] = { 7,15,23,31,39,47,55,63 };
    bool temp;

    for (int i : eighthColumArr)
    {
        if (tile == i)
        {
            temp = true;
            break;
        }
        else {
            temp = false;
        }
    }

    return temp && (offset == 1 || offset == -7 || offset == 9);
}

std::vector<int> Queen::GetLegalMoves() {

    Piece destinationTile(arrOfChess);
    int possibleDestinationTile;
    std::vector<int> legalMoves;
    for (int offset : CANDIDATE_MOVE_COORDINATES)
    {
        possibleDestinationTile = this->pieceTile;

        /*        if (firstColum(possibleDestinationTile, offset) || eighthColum(possibleDestinationTile, offset))
                {
                    continue;
                }*/
        while (IsValidTileCoordinate(possibleDestinationTile)) {

            int temp = possibleDestinationTile;
            possibleDestinationTile += offset;
            if (FirstColum(temp, offset) || EighthColum(temp, offset))
            {
                continue;
            }

            if (IsValidTileCoordinate(possibleDestinationTile))
            {
                if (!destinationTile.IsTileOccupied(possibleDestinationTile))
                {
                    legalMoves.push_back(possibleDestinationTile);
                }
                else
                {
                    if (!(this->alliance == destinationTile.PieceAlliance(possibleDestinationTile, alliance)))
                    {
                        legalMoves.push_back(possibleDestinationTile);
                    }
                    break;
                }
            }
        }
    }

    return legalMoves;
}

Rook::Rook(const int Tile, std::string alliance, int chess[]) {
    for (int i = 0; i < 64; ++i) {
        arrOfChess[i] = chess[i];
    }
    this->pieceTile = Tile;
    this->alliance = alliance;
}

bool Rook::IsValidTileCoordinate(int currentCandidate) {
    return currentCandidate >= 0 && currentCandidate < 64;
}

bool Rook::FirstColum(int tile, int offset) {
    int firstColumArr[] = { 0,8,16,24,32,40,43,56 };
    bool temp;

    for (int i : firstColumArr)
    {
        if (tile == i)
        {
            temp = true;
            break;
        }
        else {
            temp = false;
        }
    }

    return temp && (offset == -1);
}

bool Rook::EighthColum(int tile, int offset) {
    int eighthColumArr[] = { 7,15,23,31,39,47,55,63 };
    bool temp;

    for (int i : eighthColumArr)
    {
        if (tile == i)
        {
            temp = true;
            break;
        }
        else {
            temp = false;
        }
    }

    return temp && (offset == 1);
}

//std::vector<int> rook::getLegalMoves() {
//
//    Piece destinationTile(arrOfChess);
//    int possibleDestinationTile;
//    std::vector<int> legalMoves;
//    for(int offset:CANDIDATE_MOVE_COORDINATES)
//    {
//        possibleDestinationTile = this->pieceTile;
//
//        if (firstColum(possibleDestinationTile, offset) || eighthColum(possibleDestinationTile, offset))
//        {
//            continue;
//        }
//
//        while(isValidTileCoordinate(possibleDestinationTile)) {
//            if (firstColum(possibleDestinationTile, offset) || eighthColum(possibleDestinationTile, offset))
//            {
//                break;
//            }
//
//            possibleDestinationTile += offset;
//            if (isValidTileCoordinate(possibleDestinationTile))
//            {
//                if (!destinationTile.isTileOccupied(possibleDestinationTile))
//                {
//                    legalMoves.push_back(possibleDestinationTile);
//                }
//                else
//                {
//                    if(!(this->alliance == destinationTile.pieceAlliance(possibleDestinationTile, alliance)))
//                    {
//                        legalMoves.push_back(possibleDestinationTile);
//                    }
//                    break;
//                }
//            }
//        }
//    }
//
//    return legalMoves;
//}

std::vector<int> Rook::GetLegalMoves() {

    Piece destinationTile(arrOfChess);
    int possibleDestinationTile;
    std::vector<int> legalMoves;
    for (int offset : CANDIDATE_MOVE_COORDINATES)
    {
        possibleDestinationTile = this->pieceTile;

        if (FirstColum(possibleDestinationTile, offset) || EighthColum(possibleDestinationTile, offset))
        {
            continue;
        }
        while (IsValidTileCoordinate(possibleDestinationTile)) {
            int temp = possibleDestinationTile;
            possibleDestinationTile += offset;
            if (FirstColum(temp, offset) || EighthColum(temp, offset))
            {
                continue;
            }
            if (IsValidTileCoordinate(possibleDestinationTile))
            {
                if (!destinationTile.IsTileOccupied(possibleDestinationTile))
                {
                    legalMoves.push_back(possibleDestinationTile);
                }
                else
                {
                    if (!(this->alliance == destinationTile.PieceAlliance(possibleDestinationTile, this->alliance)))
                    {
                        legalMoves.push_back(possibleDestinationTile);
                    }
                    break;
                }
            }
        }
    }

    return legalMoves;
}




Bishop::Bishop(int pieceTile, std::string alliance, int chess[]) {
    for (int i = 0; i < 64; ++i) {
        arrOfChess[i] = chess[i];
    }
    this->pieceTile = pieceTile;
    this->alliance = alliance;
}

bool Bishop::IsValidTileCoordinate(int currentCandidate) {
    return currentCandidate >= 0 && currentCandidate < 64;
}

bool Bishop::FirstColum(int pieceTile, int offset) {
    int firstColumArr[] = { 0,8,16,24,32,40,43,56 };
    bool temp = false;

    for (int i : firstColumArr)
    {
        if (pieceTile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;
    }

    return temp && (offset == 7 || offset == -9);
}

bool Bishop::EighthColum(int pieceTile, int offset) {
    int eighthColumArr[] = { 7,15,23,31,39,47,55,63 };
    bool temp = false;

    for (int i : eighthColumArr)
    {
        if (pieceTile == i)
        {
            temp = true;
            break;
        }
        else
            temp = false;
    }

    return temp && (offset == -7 || offset == 9);
}

std::vector<int> Bishop::GetLegalMoves() {

    Piece destinationTile(arrOfChess);
    int possibleDestinationTile;
    std::vector<int> legalMoves;
    for (int offset : CANDIDATE_MOVE_COORDINATES)
    {
        possibleDestinationTile = this->pieceTile;

        if (FirstColum(possibleDestinationTile, offset) || EighthColum(possibleDestinationTile, offset))
        {
            continue;
        }
        while (IsValidTileCoordinate(possibleDestinationTile)) {
            int temp = possibleDestinationTile;
            possibleDestinationTile += offset;
            if (FirstColum(temp, offset) || EighthColum(temp, offset))
            {
                continue;
            }
            if (IsValidTileCoordinate(possibleDestinationTile))
            {
                if (!destinationTile.IsTileOccupied(possibleDestinationTile))
                {
                    legalMoves.push_back(possibleDestinationTile);
                }
                else
                {
                    if (!(this->alliance == destinationTile.PieceAlliance(possibleDestinationTile, this->alliance)))
                    {
                        legalMoves.push_back(possibleDestinationTile);
                    }
                    break;
                }
            }
        }
    }

    return legalMoves;
}

bool Identity::ChessKing(int initialTile, int finalTile, std::string alliance, int* arrOfChess) {
    king king1(initialTile, alliance, arrOfChess);

    std::vector<int> v1;
    v1 = king1.GetLegalMoves();


    for (std::vector<int>::iterator it1 = v1.begin(); it1 != v1.end(); ++it1) {
        if (*it1 == finalTile)
            return true;
    }

    return false;
}

bool Identity::ChessQueen(int initialTile, int finalTile, std::string alliance, int* arrOfChess) {

    Queen queen1(initialTile, alliance, arrOfChess);

    std::vector<int>v2;
    v2 = queen1.GetLegalMoves();


    for (std::vector<int>::iterator it2 = v2.begin(); it2 != v2.end(); ++it2) {
        if (*it2 == finalTile)
            return true;
    }

    return false;
}

bool Identity::ChessPawn(int initialTile, int finalTile, std::string alliance, int* arrOfChess) {
    pawn pawn1(initialTile, alliance, arrOfChess);

    std::vector<int>v3;
    v3 = pawn1.GetLegalMoves();


    for (std::vector<int>::iterator it3 = v3.begin(); it3 != v3.end(); ++it3) {
        if (*it3 == finalTile)
            return true;
    }

    return false;
}

bool Identity::ChessRook(int initialTile, int finalTile, std::string alliance, int* arrOfChess) {
    Rook rook1(initialTile, alliance, arrOfChess);

    std::vector<int> v4;
    v4 = rook1.GetLegalMoves();


    for (std::vector<int>::iterator it4 = v4.begin(); it4 != v4.end(); ++it4) {
        if (*it4 == finalTile)
            return true;
    }

    return false;
}

bool Identity::ChessBishop(int initialTile, int finalTile, std::string alliance, int* arrOfChess) {
    Bishop bishop1(initialTile, alliance, arrOfChess);

    std::vector<int> v5;
    v5 = bishop1.GetLegalMoves();


    for (std::vector<int>::iterator it5 = v5.begin(); it5 != v5.end(); ++it5) {
        if (*it5 == finalTile)
            return true;
    }

    return false;
}

bool Identity::ChessKnight(int initialTile, int finalTile, std::string alliance, int* arrOfChess) {
    knight knight1(initialTile, alliance, arrOfChess);

    std::vector<int> v6;
    v6 = knight1.GetLegalMoves();


    for (std::vector<int>::iterator it6 = v6.begin(); it6 != v6.end(); ++it6) {
        if (*it6 == finalTile)
            return true;
    }

    return false;
}

std::string Identity::CheckAlliance(int piece) {
    if (piece > 0)
        return "white";
    else if (piece < 0)
        return "black";
}

bool Identity::Identifier(int initialTile, int finalTile, int piece, int* arrOfChess)
{
    std::string alliance = CheckAlliance(piece);

    if (piece == -1 || piece == 1)
        return ChessRook(initialTile, finalTile, alliance, arrOfChess);

    else if (piece == -2 || piece == 2)
        return ChessKnight(initialTile, finalTile, alliance, arrOfChess);

    else if (piece == -3 || piece == 3)
        return ChessBishop(initialTile, finalTile, alliance, arrOfChess);

    else if (piece == -4 || piece == 4)
        return ChessQueen(initialTile, finalTile, alliance, arrOfChess);

    else if (piece == -5 || piece == 5)
        return ChessKing(initialTile, finalTile, alliance, arrOfChess);

    else if (piece == -6 || piece == 6)
        return ChessPawn(initialTile, finalTile, alliance, arrOfChess);

}

