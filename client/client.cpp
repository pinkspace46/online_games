#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 1024
#define PORT 12345
#define MAX_NAME_LENGTH 20
#define MAX_GAME_TYPE 2

int main(int argc, char *argv[])
{
    int client_socket_fd;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);
    
    int addrlen = sizeof(address);
    
    char buffer[BUFFSIZE], player_name[MAX_NAME_LENGTH];
    int recv_signal, send_signal, game_type;
    int const welcome_msg_num = 777;
    
    //create socket
	if ((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Create socket failed.\n");
        exit(EXIT_FAILURE);
	}
    
    //connect with server
    if (connect(client_socket_fd, (struct sockaddr *)&address, addrlen) == -1) {
        perror("Connection failed\n");
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "Connected to server.\n";
    }
    
    //receive welcome message number
    recv(client_socket_fd, &recv_signal, sizeof(recv_signal), 0);
    if (recv_signal != welcome_msg_num) {
        perror("Received welcome message incorrect\n");
    }
    else {
        std::cout << "Hello there!\n";
    }
    
    //enter player name
    while (true) {
        std::cout << "Enter player name. (Less than 20 characters)\n";
        std::cin >> player_name;
        if (strlen(player_name) > MAX_NAME_LENGTH) {
            std::cout << "Player name should be less than 20 characters.\n";
            continue;
        }
        
        send_signal = 1; // send player name signal
        send(client_socket_fd, &send_signal, sizeof(send_signal), 0);
        
        if (send(client_socket_fd, player_name, strlen(player_name), 0) != strlen(player_name)) {
            perror("Player name not sent.\n");
        }
        else {
            std::cout << "Player name sent.\n";
        }

        recv(client_socket_fd, &recv_signal, sizeof(recv_signal), 0); //receive signal of whether name repeated
        
        if (recv_signal == 0) {
            std::cout << "Player name has already been used.\n";
        }
        else if (recv_signal == 1) {
            std::cout << "Welcome " << player_name << "!\n";
            break;
        }
        else {
            perror("incorrect signal\n");
        }
    }
    
    //choose a game
    std::cout << "Choose a game.(Enter game number)\n" << "1. Tic tac toe\n" << "2. Gobang\n";
    while (true) {
        std::cin >> game_type; // type of game
        if (game_type > 0 && game_type < MAX_GAME_TYPE + 1) {
            break;
        }
        else {
            std::cout << "Invalid game number. Try again!\n";
        }
    }
    
    //send game request to server
    send_signal = 2; //send game request signal
    send(client_socket_fd, &send_signal, sizeof(send_signal), 0);
    send(client_socket_fd, &game_type, sizeof(game_type), 0); // send game number to server
    std::cout << "Waiting for other players...\n";
    
    recv(client_socket_fd, &recv_signal, sizeof(recv_signal), 0);
    if (recv_signal == 2) {
        system("clear");
        std::cout << "Game starts!\n";
    }
    else {
        perror("incorrect signal\n");
    }
    
    //enter game
    
    
    close(client_socket_fd);
}
