#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include "game.hpp"

#define MAX_PLAYERS 10

class tic_tac_toe: public game {
    private:
        char board[19];
        
    public:
        tic_tac_toe();
        void game_initialize();
        char* process_move(char* message);
        int check_game();
        char* get_board();
};

#endif // TIC_TAC_TOE_H
