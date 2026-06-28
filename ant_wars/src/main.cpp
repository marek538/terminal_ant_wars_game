#include <iostream>
#include "game_handler.h"
#include <cassert>

using namespace std;


int main()
{
    try
    {

        CGameHandler game;
        game.playGame();
    }
    catch(exception &input)
    {
        cerr << input.what();
    }



    return 0;
}