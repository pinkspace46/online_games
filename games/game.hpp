#ifndef GAME_H
#define GAME_H

#define MAX_PLAYERS 10

class game {
    private:
        bool state; //in game or not
        int tablenum;

    public:
        int player_fd[MAX_PLAYERS]; // should be private, temporarily set to public
        game();
        void add_player(int fd, int player_order);
};

#endif // GAME_H
