#ifndef GOMOKU_H
#define GOMOKU_H

#include "game.hpp"

#define BOARD_SIZE 19

class gomoku: public game {
    private:
        char board[BOARD_SIZE][BOARD_SIZE];
    public:
        gomoku();
        char* process_move(char* message);
        char* get_board();
};

#endif // GOMOKU_H
