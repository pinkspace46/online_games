#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include "game.hpp"

#define MAX_PLAYERS 10

class tic_tac_toe: public game {
    private:
        char board[3][3];
        
    public:
        tic_tac_toe();
        void set_symbol();
        void play_move(int row, int column, char symbol);
        int check_game();
        void print_board();
};

#endif // TIC_TAC_TOE_H
