#ifndef GAME_H
#define GAME_H

#define MAX_PLAYERS 10;

class game {
    private:
        bool state;
        int tablenum;
        int players[MAX_PLAYERS];
        
    public:
        game();
};

#endif // GAME_H
