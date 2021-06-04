#include "lobby_manager.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

// usage of arpa/inet.h here should be removed afterwards,
// we shouldn't be dealing with ip address and ports here,
// as they are business of the server main function.

lobby_manager::lobby_manager() : buffer{}
{
    number_of_players = 0;
    number_of_tables = 0;
}

int lobby_manager::notify(int fd)
{
    int valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    recv(fd, &recv_signal, sizeof(recv_signal), 0); //receive signal from client
    
    getpeername(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    std::cout << "Received signal " << recv_signal << " from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
    
    switch (recv_signal) {
        case 0: //closing
            return fd;
        case 1: // receiving player name
            valread = read(fd, buffer, BUF_SIZE);
            buffer[valread] = '\0';

            // check if name repeats
            // and other stuff
            
            return (-1);

        case 2: //receiving game request
            
            break;
            
        default:
            break;
    }
}

bool lobby_manager::repeat_name(char*)
{
    bool repeat = false;
    return repeat;
}

void lobby_manager::create_game(int fd, int game_type)
{
    number_of_tables += 1;
    switch (game_type) {
        case 1:
            tic_tac_toe table;
            game_list_1.add();
            break;
            
        default:
            break;
    }
}

void lobby_manager::close_game(game* game_ptr)
{
    
}
