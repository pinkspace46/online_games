#ifndef GAME_H
#define GAME_H

class game {
    private:
        int* player_fd;
        int current_players;
    protected:
        int game_players; // amount of player to start game
        int active_player; // player_fd index
    public:
        game(int player_count);
        void add_player(int fd);
        int get_current_player_count();
        const int* get_player_fd() const;
        bool is_full();
        int get_active_player();
        virtual char* process_move(char* message) = 0;
        
};

#endif // GAME_H
