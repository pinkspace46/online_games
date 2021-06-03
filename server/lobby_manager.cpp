#include "lobby_manager.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

// usage of arpa/inet.h here should be removed afterwards,
// we shouldn't be dealing with ip address and ports here,
// as they are business of the server main function.

lobby_manager::lobby_manager() {
    number_of_players = 0;
    number_of_tables = 0;
    buffer[0] = '\0';
    for (auto i : player_names) {
        player_names[i] = '\0';
    }
    for (auto i : table_state) {
        table_state[i] = false;
    }
}

int lobby_manager::notify(int fd) {
    int valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    recv(fd, r_signal, sizeof(r_signal), 0); //receive signal from client
    
    getpeername(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    std::cout << "Received signal " << r_signal << " from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
    
    switch (*r_signal) {
        case 0: //closing
            return fd;
        case 1: // receiving player name
            valread = read(fd, buffer, BUF_SIZE));
            buffer[valread] = '\0';
            
            if (repeat_name()) {
                *s_signal = 0;
                send(fd, s_signal, sizeof(s_signal), 0);
            }
            else {
                *s_signal = 1;
                player_names[number_of_players] = buffer;
                number_of_players += 1;
                send(fd, s_signal, sizeof(s_signal), 0);
            }
            return (-1);
        case 2: //receiving game request
            
            break;
            
        default:
            break;
    }
    
    
    /*if ((valread = read(fd, buffer, BUF_SIZE)) == 0) {
        return fd;
    }
    else {
        getpeername(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        buffer[valread] = '\0';
        std::cout << "Received \"\n" << buffer << "\" from " << inet_ntoa(address.sin_addr) << ":"
                  << ntohs(address.sin_port) << std::endl;
        //send(fd, buffer, strlen(buffer), 0);
        return -1;
    }*/
}

bool lobby_manager::repeat_name() {
    bool repeat = false;
    for (int i = 0; i < number_of_players; ++i) {
        if (strlen(player_names[i]) == strlen(buffer)) {
            repeat = true;
            for (int j = 0; j < strlen(buffer); ++j) {
                if (player_names[i][j] != buffer[j]) {
                    repeat = false;
                    break;
                }
            }
            if (repeat) {
                return repeat; //true
            }
        }
    }
    return repeat; //false
}

void lobby_manager::create_game(int gamenum) {
    number_of_tables += 1;
    switch (gamenum) {
        case 1:
            tic_tac_toe table;
            game_list_1.add();
            break;
            
        default:
            break;
    }
}

void lobby_manager::close_game() {
    
}
