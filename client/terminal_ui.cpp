#include <iostream>
#include <string.h>
#include <map>
#include "games/game_types.hpp"

std::map<int, std::string> game_types = {{TIC_TAC_TOE, "tic tac toe"}, {GOMOKU, "gomoku"}};

void get_player_name(char* buf, int size)
{
    while(true) {
        std::cout << "Enter player name. (Less than 20 characters)\n";
        std::cin >> buf;
        if (strlen(buf) > size) {
            std::cout << "Player name should be less than 20 characters.\n";
            continue;
        }
        return;
    }
}

void show_name_used()
{
    std::cout << "Player name has already been used.\n";
}

void show_welcome_player(char* buf)
{
    std::cout << "Welcome " << buf << "!\n";
}

int get_game_type()
{
    std::cout << "Choose a game.(Enter game number)\n";
    for (auto const& x : game_types) {
        std::cout << x.first
                  << ':'
                  << x.second
                  << std::endl;
    }
    int game_type;
    while (true) {
        std::cin >> game_type; // type of game
        if (game_type >= 0 && game_type < game_types.size()) {
            break;
        }
        else {
            std::cout << "Invalid game number. Try again!\n";
        }
    }
    return game_type;
}

void show_waiting()
{
    std::cout << "Waiting for other players...\n";
}

void show_game_start()
{
    system("clear");
    std::cout << "Game starts!\n";
}

