#pragma once 
#include <SFML\Network.hpp>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <algorithm>
#include <mutex>

class TCPSocketManager
{
public:
    enum MessageTypes
    {
        LOGIN,
        MESSAGE,
        START_GAME,
        MAKE_MOVE,
        MOVE_RESPONSE,
        UPDATE_GAME,
        GAME_CLOSE,
        CONTINUE,
        DISCONNECT,
        COUNT
    };
};
