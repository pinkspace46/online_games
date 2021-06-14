#include "lobby_manager.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

// usage of arpa/inet.h here should be removed afterwards,
// we shouldn't be dealing with ip address and ports here,
// as they are business of the server main function.

lobby_manager::lobby_manager()
{
    recv_signal = 0;
    send_signal = 0;
    number_of_players = 0;
    number_of_tables = 0;
    for (int i = 0; i < BUF_SIZE; ++i) {
        buffer[i] = '\0';
    }
    for (int i = 0; i < MAX_GAME_TYPE; ++i) {
        waiting_game[i] = NULL;
    }
}

int lobby_manager::notify(int fd)
{
    int valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    valread = recv(fd, &recv_signal, sizeof(recv_signal), 0); // receive signal from client
    // read nothing means the other end had closed
    if (valread == 0) {
        return fd;
    }
    
    getpeername(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    std::cout << "Received signal " << recv_signal << " from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;
    
    switch (recv_signal) {
        case 0: // reserved
            return 0;
        case 1: // receive player name signal
            valread = recv(fd, buffer, BUF_SIZE, 0); // receive player name
            buffer[valread] = '\0';
            std::cout << "Received player name: " << buffer << std::endl;
            if (repeat_name(buffer)) {
                send_signal = 0; // repeated name signal (fail)
                if (send(fd, &send_signal, sizeof(send_signal), 0) != sizeof(send_signal)) {
                    perror("send");
                }
            }
            else {
                fd_name_mapping[fd] = buffer;
                send_signal = 1; // no repeated name signal (success)
                if (send(fd, &send_signal, sizeof(send_signal), 0) != sizeof(send_signal)) {
                    perror("send");
                }
            }
            break;
        case 2: //receive game request signal
            int game_type;
            bool ready;
            valread = recv(fd, &game_type, sizeof(game_type), 0); // receive game_type
            game_type -= 1; // make game_type start from 0
            ready = join_game(fd, game_type);
            if (ready) {
                send_signal = 2; // game start signal
                for (int i = 0; i < 2; ++i) { // the '2' needs to be modified if more than 2 player games
                    send((fd_game_mapping[fd]->player_fd)[i], &send_signal, sizeof(send_signal), 0);
                }
            }
            break;
        case 3: // receive game move signal
            break;
        default:
            break;
    }
    return -1;
}

bool lobby_manager::repeat_name(char* name)
{
    bool repeat = false;
    std::map<int, char*>::iterator it;
    for (it = fd_name_mapping.begin(); it != fd_name_mapping.end(); ++it) {
        if (strlen(name) == strlen(it->second)) {
            repeat = true;
            for (int i = 0; i < strlen(name); ++i) {
                if (*(name + i) != *(it->second + i)) {
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

bool lobby_manager::join_game(int fd, int game_type) //return game ready to start or not
{
    if (waiting_game[game_type] == NULL) {
        game* new_game_ptr = create_game(fd, game_type);
        waiting_game[game_type] = new_game_ptr;
        return false;
    }
    else{
        // need to be modified if more than 2 player games
        fd_game_mapping[fd] = waiting_game[game_type];
        waiting_game[game_type]->add_player(fd, 2);
        waiting_game[game_type] = NULL;
        return true;
    }
}

game* lobby_manager::create_game(int fd, int game_type)
{
    if (game_type == 0){
        tic_tac_toe* ttt;
        ttt->add_player(fd, 1);
        fd_game_mapping[fd] = ttt;
        return ttt;
    }
}

void lobby_manager::close_game(game* game_ptr)
{
    
}
