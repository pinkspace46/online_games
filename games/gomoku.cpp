#include "games/gomoku.hpp"
#include <iostream>
#include <stdlib.h>
#include <string.h>

gomoku::gomoku() : game(2)
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) board[i][j] = ' ';
    }
}

char* gomoku::process_move(char* message)
{
    
}

char* gomoku::get_board()
{
    return board;
}
