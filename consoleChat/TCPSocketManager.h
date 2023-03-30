#pragma once 
#include <SFML\Network.hpp>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <algorithm>

class TCPSocketManager
{
public:
    enum MessageTypes
    {
        LOGIN,
        MESSAGE,
        START_GAME,
        MAKE_MOVE,
        DISCONNECT,
        COUNT
    };
};
