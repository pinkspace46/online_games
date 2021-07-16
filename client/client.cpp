#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "games/game_types.hpp"
#include "signals.hpp"
#include "send_recv.hpp"

#define BUF_SIZE 1024
#define PORT 12345
#define MAX_NAME_LENGTH 20
#define MAX_GAME_TYPE 2

void get_player_name(char*, int);
void show_name_used();
void show_welcome_player(char*);
int get_game_type();
void show_waiting();
void show_game_start();

int send_name(int client_socket_fd, char* buf)
{
    int send_signal, recv_signal;
    send_signal = PLAYER_NAME; // send player name signal
    SEND(client_socket_fd, &send_signal, sizeof(send_signal), 0);
    
    SEND(client_socket_fd, buf, strlen(buf), 0);
    std::cerr << "Player name sent.\n";

    RECV(client_socket_fd, &recv_signal, sizeof(recv_signal), 0); //receive signal of whether name repeated
    
    if (recv_signal == PLAYER_NAME_FAIL) {
        show_name_used();
        return PLAYER_NAME_FAIL;
    }
    else if (recv_signal == PLAYER_NAME_SUCC) {
        show_welcome_player(buf);
        return PLAYER_NAME_SUCC;
    }
    else {
        std::cerr << "Incorrect signal\n";
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int client_socket_fd;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);
    
    int addrlen = sizeof(address);
    
    char buffer[BUF_SIZE], player_name[MAX_NAME_LENGTH];
    int recv_signal, send_signal, game_type;
    
    // create socket
	if ((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
        exit(EXIT_FAILURE);
	}
    
    // connect with server
    if (connect(client_socket_fd, (struct sockaddr *)&address, addrlen) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "Connected to server.\n";
    }
    
    int valread;
    // receive welcome message
    valread = RECV(client_socket_fd, &buffer, BUF_SIZE, 0);
    buffer[valread] = '\0';
    for (int i = 0; i < valread; i++) {
        std::cout << buffer[i];
    }
    
    // enter player name
    while (true) {
        get_player_name(player_name, MAX_NAME_LENGTH);
        if (send_name(client_socket_fd, player_name))
            break;
    }
    
    //choose a game
    game_type = get_game_type();
    
    //send game request to server
    send_signal = GAME_REQUEST; //send game request signal
    SEND(client_socket_fd, &send_signal, sizeof(send_signal), 0);
    SEND(client_socket_fd, &game_type, sizeof(game_type), 0); // send game number to server
    show_waiting();
    
    RECV(client_socket_fd, &recv_signal, sizeof(recv_signal), 0);
    if (recv_signal == GAME_START) {
        show_game_start();
    }
    else {
        std::cerr << "Incorrect signal\nn";
        exit(EXIT_FAILURE);
    }
    
    //enter game
    switch (game_type) {
        case TIC_TAC_TOE:
            while (true) {
                recv(client_socket_fd, &recv_signal, sizeof(recv_signal), 0);
                if (recv_signal == ACTIVE_PLAYER) { //your turn
                    std::cout << "Your turn!\n";
                    char* message;
                    message = new char[3];
                    std::cin >> message;
                    send_signal = GAME_MOVE; //send game move signal
                    SEND(client_socket_fd, &send_signal, sizeof(send_signal), 0);
                    SEND(client_socket_fd, message, strlen(message), 0);
                    delete[] message;
                }
                else if (recv_signal == NON_ACTIVE_PLAYER) { // opponent's turn
                    std::cout << "Opponent\'s turn!\n";
                }
                else if (recv_signal == GAME_STATE) { //receive message about game
                    valread = RECV(client_socket_fd, buffer, BUF_SIZE, 0);
                    buffer[valread] = '\0';
                    std::cout << buffer;
                }
            }
            break;
        case GOBANG:
            break;
        default:
            break;
    }
    close(client_socket_fd);
}
