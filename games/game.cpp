#include "games/game.hpp"

game::game(int player_count)
{
    player_fd = new int[player_count]; //delete?
    game_players = player_count;
    current_players = 0;
    active_player_index = 0;
}

void game::add_player(int fd)
{
    player_fd[current_players] = fd;
    current_players++;
}

int game::get_current_player_count()
{
    return current_players;
}

const int* game::get_player_fd() const
{
    return player_fd;
}

bool game::is_full() 
{
    return game_players == current_players;
}

int game::get_active_player_index()
{
    return active_player_index;
}
