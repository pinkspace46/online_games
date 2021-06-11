#include "tic_tac_toe.hpp"
#include <iostream>
#include <stdlib.h>

tic_tac_toe::tic_tac_toe()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = ' ';
        }
    }
}

void tic_tac_toe::game_initialize()
{
    
}

void tic_tac_toe::play_move(int row, int column, char symbol) {
    board[row][column] = symbol;
}

int tic_tac_toe::check_game() {
    
}

void tic_tac_toe::print_board() {
    system("clear");
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}
