#pragma once
#include <SFML/Graphics.hpp>
#include <SFML\Network.hpp>
#include <iostream>
#include <fstream>
#include "Pieces.h"
class TCPClient;

using namespace sf;
using namespace std;


class ChessBoard
{
private:
    const float WIDTH = 650;
    const float HEIGTH = 650;
    int spritepositions[64] = { 0,1,2,3,4,5,6,7,
                                8,9,10,11,12,13,14,15,
                                64,64,64,64,64,64,64,64,
                                64,64,64,64,64,64,64,64,
                                64,64,64,64,64,64,64,64,
                                64,64,64,64,64,64,64,64,
                                48,49,50,51,52,53,54,55,
                                56,57,58,59,60,61,62,63 };

    int board[64] = { -1,-2,-3,-4,-5,-3,-2,-1,
                    -6,-6,-6,-6,-6,-6,-6,-6,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0,
                    6, 6, 6, 6, 6, 6, 6, 6,
                    1, 2, 3, 4, 5, 3, 2, 1 };


    sf::RectangleShape rectangle[64];
    sf::Sprite sprite[65];
    int initTile;
    int q[64];

public:
    struct ExternalUpdateData
    {
        int initialTile;
        int finalTile;
        int piece;
    };
    ExternalUpdateData externalUpdateData;

    int turn = 1;
    void LoadTextures(Texture texture[64]);
    void LoadBoard(Texture texture[64], RectangleShape rectangle[64], Sprite sprite[64]);
    bool UpdateBoard(int n, int j, RectangleShape rectangle[64], Sprite sprite[65]);
    void Run(TCPClient* client);
};
