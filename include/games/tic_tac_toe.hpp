#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include "game.hpp"

class tic_tac_toe: public game {
    private:
        char board[19];
        
    public:
        tic_tac_toe();
        char* process_move(char* message);
        char* get_board();
};

#endif // TIC_TAC_TOE_H
