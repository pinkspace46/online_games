#ifndef GAME_H
#define GAME_H

class game {
    private:
        bool state; // in game or not
        int* player_fd;
        int current_players;
        int game_players; // amount of player to start game

    public:
        game(int player_count);
        void add_player(int fd);
        int get_current_player_count();
        const int* get_player_fd() const;
        bool is_full();
};

#endif // GAME_H
