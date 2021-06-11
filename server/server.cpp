#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <assert.h>

#include "../utils/doubly_linked_list.hpp"
#include "lobby_manager.hpp"

#define PORT 12345
#define MAX_PENDING_CONNS 50

int main(int argc, char *argv[])
{
    int main_socket_fd;
    int new_client_socket_fd;
    int opt = 1;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    int addrlen = sizeof(address);

    int const welcome_msg_num = 777;
    int valread;

    fd_set fd_bitmap;

    doubly_linked_list<int> connection_fds;

    lobby_manager lm;

    // create main socket
    if ((main_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set main socket to ignore TIME_WAIT when restarting
    if (setsockopt(main_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // bind the socket to localhost and PORT
    if (bind(main_socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen on the socket
    if (listen(main_socket_fd, MAX_PENDING_CONNS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    puts("start listening for connections ...");

    while (true) {
        int max_fd;
        int ready_fd_count;
        // clear fd bitmap
        FD_ZERO(&fd_bitmap);
        // add main socket fd to set
        FD_SET(main_socket_fd, &fd_bitmap);
        max_fd = main_socket_fd;

        // add existing connections' fds to set
        // and update max_fd if needed
        for (auto i : connection_fds) {
            FD_SET(i, &fd_bitmap);
            if (i > max_fd) {
                max_fd = i;
            }
        }

        // wait for something to read
        std::cout << "select" << std::endl;
        ready_fd_count = select(max_fd + 1, &fd_bitmap, NULL, NULL, NULL);
        if ((ready_fd_count < 0) && (errno != EINTR)) {
            perror("select error");
        }

        // check main socket fd for new connections
        if (FD_ISSET(main_socket_fd, &fd_bitmap)) {
            if ((new_client_socket_fd = accept(main_socket_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            std::cout << "New connection, socket fd: " << new_client_socket_fd << ", ip: " << inet_ntoa(address.sin_addr) 
                      << ", port: " << ntohs(address.sin_port) << std::endl;

            // send welcome message number 777
            if (send(new_client_socket_fd, &welcome_msg_num, sizeof(welcome_msg_num), 0) != sizeof(welcome_msg_num)) {
                perror("send");
            }
            std::cout << "Welcome message number sent" << std::endl;
            
            // add new fd to fd list
            connection_fds.add(new_client_socket_fd);
            std::cout << "Add new client socket fd: " << new_client_socket_fd << std::endl;
        }

        // check other client sockets
        for (auto it = connection_fds.begin(), end = connection_fds.end(); it != end; ++it) {
            const auto i = *it;
            if (FD_ISSET(i, &fd_bitmap)) {
                // returns the fd itself if it is closing
                int fd_to_close = lm.notify(i);
                assert(fd_to_close == i || fd_to_close == -1);
                // closing
                if (fd_to_close == i) {
                    getpeername(i, (struct sockaddr *)&address, (socklen_t*)&addrlen);
                    std::cout << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << " disconnected" << std::endl; 
                    close(i);
                    --it;
                    connection_fds.remove(i);
                }
            }
        }
    }

    return 0;
}
