#ifndef LOBBY_MANAGER_H
#define LOBBY_MANAGER_H

#include <map>
#include "../games/game.hpp"
#include "../games/tic_tac_toe.hpp"

#define BUF_SIZE 1024
#define MAX_GAME_TYPE 2
#define MAX_TABLES 20
#define MAX_USERS 40

class lobby_manager {
    private:
        char buffer[BUF_SIZE];
        game* waiting_game[MAX_GAME_TYPE];
        std::map<int, game*> fd_game_mapping;
        std::map<int, char*> fd_name_mapping;
        int number_of_players, number_of_tables;
        int recv_signal, send_signal; // receive signal, send signal
        bool repeat_name(char*); //check whether player name is repeated
        bool join_game(int fd, int game_type);
        game* create_game(int fd, int game_type);
        void close_game(game* game_ptr);
        
    public:
        lobby_manager();
        int notify(int fd); // read signal from client and process
};

#endif // LOBBY_MANAGER_H 
