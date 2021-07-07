#include "tic_tac_toe.hpp"
#include <iostream>
#include <stdlib.h>
#include <string.h>

tic_tac_toe::tic_tac_toe() : game(2)
{
    for (int i = 0; i < 18; ++i) {
        board[i] = ' ';
    }
    for (int i = 0; i < 9; ++i) {
        board[2 * i + 1] = '|';
    }
    for (int i = 0; i < 3; ++i) {
        board[6 * i + 5] = '\n';
    }
    board[18] = '\0';
}

char* tic_tac_toe::process_move(char* message)
{
    int r = message[0] - '0';
    int c = message[1] - '0';
    board[6 * r + 2 * c] = message[2];
    strcpy(message, board);
    active_player = (active_player + 1) % game_players; // change next active_player
    return message;
}

int tic_tac_toe::check_game()
{
    
}

char* tic_tac_toe::get_board()
{
    return board;
}
