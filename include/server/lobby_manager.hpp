#ifndef LOBBY_MANAGER_H
#define LOBBY_MANAGER_H

#include <map>
#include "../games/game.hpp"
#include "../games/tic_tac_toe.hpp"
#include "../games/gomoku.hpp"

#define BUF_SIZE 1024
#define GAME_TYPES 2

struct player {
    game* game_ptr;
    char* name;
};

class lobby_manager {
    private:
        char buffer[BUF_SIZE];
        game* waiting_game[GAME_TYPES];
        std::map<int, player*> fd_player_mapping;
        int recv_signal, send_signal; // receive signal, send signal
        bool repeat_name(char*); //check whether player name is repeated
        bool join_game(int fd, int game_type);
        game* create_game(int fd, int game_type);
        void send_next_turn(int fd);
        void terminate_game(game* game_ptr);
    public:
        lobby_manager();
        int notify(int fd); // read signal from client and process
};

#endif // LOBBY_MANAGER_H 
