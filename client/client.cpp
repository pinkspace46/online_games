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

int main(int argc, char *argv[]) {

    int client_socket_fd, con;
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    int addrlen = sizeof(address);
    
    char buffer[BUFFSIZE], message[BUFFSIZE], player_name[MAX_NAME_LENGTH];
    int *r_signal, *s_signal, *gamenum;
    
    //create socket
	if ((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Create socket failed.\n");
        exit(EXIT_FAILURE);
	}
    
    //connect with server
    if ((con = connect(client_socket_fd, (struct sockaddr *)&address, addrlen)) == -1) {
        perror("Connection failed\n");
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "Connected to server.\n";
    }
    
    //receive welcome message
    recv(client_socket_fd, buffer, sizeof(buffer), 0);
    std::cout << buffer;
    
    //enter player name
    while (true) {
        std::cout << "Enter player name. (Less than 20 characters)\n";
        std::cin >> player_name;
        if (strlen(player_name) > MAX_NAME_LENGTH) {
            std::cout << "Player name should be less than 20 characters.\n";
            continue;
        }
        
        *s_signal = 1; // sending player name
        send(client_socket_fd, s_signal, sizeof(s_signal), 0);
        
        if (send(client_socket_fd, player_name, sizeof(player_name), 0) != sizeof(player_name)) {
            perror("Player name not sent.\n");
        }
        else {
            std::cout << "Player name sent.\n";
        }

        recv(client_socket_fd, r_signal, sizeof(r_signal), 0);
        
        if (*r_signal == 0) {
            std::cout << "Player name has already been used.\n";
            continue;
        }
        else if (*r_signal == 1) {
            std::cout << "Welcome " << player_name << "!\n";
            break;
        }
        else {
            perror("incorrect signal\n");
        }
    }
    
    //start a game
    
    std::cout << "Choose a game.\n" << "1. Tic tac toe\n" << "2. Gobang\n";
    std::cin << gamenum; // type of game
    //to do: validation of gamenum
    *s_signal = 2;
    send(client_socket_fd, s_signal, sizeof(s_signal), 0); // send game request signal to server
    send(client_socket_fd, gamenum, sizeof(gamenum), 0); // send game number to server
    std::cout << "Waiting for other players...\n";
    
    // enter game
    recv(client_socket_fd, r_signal, sizeof(r_signal), 0); //receive signal of gametype
    if (*r_signal == 2) { //tic_tac_toe
        recv(client_socket_fd, buffer, strlen(buffer), 0); // receive from server O or X
        std::cout << "You are " << buffer << ".\n";
        while (*r_signal != 3) {// your turn to play move
              
        }
    }
    
    close(client_socket_fd);
}
