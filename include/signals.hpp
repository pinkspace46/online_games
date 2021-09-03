#ifndef SIGNALS_H
#define SIGNALS_H

#define EMPTY              0
#define PLAYER_NAME        1 //send
#define PLAYER_NAME_FAIL   0 //recv
#define PLAYER_NAME_SUCC   1 //recv
#define GAME_START         2 //recv
#define GAME_REQUEST       2 //send
#define GAME_MOVE          3 //send
#define ACTIVE_PLAYER      3 //recv
#define NON_ACTIVE_PLAYER  4 //recv
#define GAME_STATE         5 //recv

#endif /* SIGNALS_H */
