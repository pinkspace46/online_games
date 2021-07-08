#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#include "server/lobby_manager.hpp"
#include "games/game_types.hpp"
#include "signals.hpp"

// usage of arpa/inet.h here should be removed afterwards,
// we shouldn't be dealing with ip address and ports here,
// as they are business of the server main function.

lobby_manager::lobby_manager()
{
    recv_signal = EMPTY;
    send_signal = EMPTY;
    for (int i = 0; i < BUF_SIZE; ++i) {
        buffer[i] = '\0';
    }
    for (int i = 0; i < GAME_TYPES; ++i) {
        waiting_game[i] = NULL;
    }
}

int lobby_manager::notify(int fd)
{
    int valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    valread = recv(fd, &recv_signal, sizeof(recv_signal), 0); // receive signal from client
    // reading nothing means the other end had closed
    if (valread == 0) {
        return fd;
    }
    
    getpeername(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    std::cout << "Received signal " << recv_signal << " from " << inet_ntoa(address.sin_addr) << ":" 
                                    << ntohs(address.sin_port) << std::endl;
    
    switch (recv_signal) {
        case 0: // reserved
            return 0;

        case PLAYER_NAME: // receive player name signal
            valread = recv(fd, buffer, BUF_SIZE, 0); // receive player name
            buffer[valread] = '\0';
            std::cout << "Received player name: " << buffer << std::endl;
            if (repeat_name(buffer)) {
                send_signal = PLAYER_NAME_FAIL; // repeated name signal (fail)
                if (send(fd, &send_signal, sizeof(send_signal), 0) != sizeof(send_signal)) {
                    perror("send");
                }
            }
            else {
                player* new_player = new struct player;
                new_player->game_ptr = NULL;
                new_player->name = new char[valread+1];
                for (int i = 0; i < valread; i++) {
                    (new_player->name)[i] = buffer[i];
                }
                (new_player->name)[valread] = '\0';
                fd_player_mapping[fd] = new_player;
                send_signal = PLAYER_NAME_SUCC; // no repeated name signal (success)
                if (send(fd, &send_signal, sizeof(send_signal), 0) != sizeof(send_signal)) {
                    perror("send");
                }
            }
            break;

        case GAME_REQUEST: // receive game request signal
            int game_type;
            valread = recv(fd, &game_type, sizeof(game_type), 0); // receive game_type
            game_type -= 1; // make game_type start from 0
            bool ready;
            ready = join_game(fd, game_type);
            if (ready) {
                send_signal = GAME_START; // game start signal
                for (int i = 0; i < (fd_player_mapping[fd])->game_ptr->get_current_player_count(); ++i) {
                    send(((fd_player_mapping[fd])->game_ptr->get_player_fd())[i], &send_signal, sizeof(send_signal), 0);
                }
                send_next_turn(fd);
            }
            break;
            
        case GAME_MOVE: // receive game move signal
            char* message;
            message = new char[1000];
            recv(fd, message, 1000, 0);
            message = (fd_player_mapping[fd])->game_ptr->process_move(message);
            send_signal = GAME_STATE; //game message signal
            for (int i = 0; i < (fd_player_mapping[fd])->game_ptr->get_current_player_count(); ++i) {
                send(((fd_player_mapping[fd])->game_ptr->get_player_fd())[i], &send_signal, sizeof(send_signal), 0);
                send(((fd_player_mapping[fd])->game_ptr->get_player_fd())[i], message, strlen(message), 0);
            }
            delete[] message;
            send_next_turn(fd);
            break;
            
        default:
            break;
    }
    return -1;
}

bool lobby_manager::repeat_name(char* name)
{
    bool repeat = false;
    std::map<int, player*>::iterator it;
    for (it = fd_player_mapping.begin(); it != fd_player_mapping.end(); ++it) {
        if (strlen(name) == strlen(it->second->name)) {
            repeat = true;
            for (int i = 0; i < strlen(name); ++i) {
                if (*(name + i) != *(it->second->name + i)) {
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
    else {
        fd_player_mapping[fd]->game_ptr = waiting_game[game_type];
        fd_player_mapping[fd]->game_ptr->add_player(fd);
        if (fd_player_mapping[fd]->game_ptr->is_full()) {
            waiting_game[game_type] = NULL;
        }
        return fd_player_mapping[fd]->game_ptr->is_full();
    }
}

game* lobby_manager::create_game(int fd, int game_type)
{
    if (game_type == TIC_TAC_TOE){
        tic_tac_toe* ttt = new tic_tac_toe();
        ttt->add_player(fd);
        fd_player_mapping[fd]->game_ptr = ttt;
        return ttt;
    }
    else {
        return NULL;
    }
}

void lobby_manager::send_next_turn(int fd)
{
    for (int i = 0; i < (fd_player_mapping[fd])->game_ptr->get_current_player_count(); ++i) {
        if (i == (fd_player_mapping[fd])->game_ptr->get_active_player_index()) {
            send_signal = ACTIVE_PLAYER; //send signal to active player
        }
        else {
            send_signal = NON_ACTIVE_PLAYER; //send signal to non-active players
        }
        send(((fd_player_mapping[fd])->game_ptr->get_player_fd())[i], &send_signal, sizeof(send_signal), 0);
    }
}

void lobby_manager::terminate_game(game* game_ptr)
{
    
}
