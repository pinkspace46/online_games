#include "game.hpp"

game::game()
{
    state = false;
    tablenum = 0;
}

void game::add_player(int fd, int player_order)
{
    player_order -= 1; // make player_order start from 0
    player_fd[player_order] = fd;
}
