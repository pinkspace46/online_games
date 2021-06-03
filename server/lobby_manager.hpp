#ifndef LOBBY_MANAGER_H
#define LOBBY_MANAGER_H

#define BUF_SIZE 1024
#define MAX_TABLES 20
#define MAX_PLAYERS 40

class lobby_manager {
    private:
        char buffer[BUF_SIZE];
        char* player_names[MAX_PLAYERS];
        bool table_state[MAX_TABLES];
        int number_of_players, number_of_tables;
        int *r_signal, *s_signal; // receive signal, send signal
        
        
    public:
        lobby_manager();
        int notify(int fd); // read signal from client and process
        bool repeat_name(); //check whether player name is repeated
        void create_game(int gamenum);
        void close_game(int gamenum);
};

#endif // LOBBY_MANAGER_H 
