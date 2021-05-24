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
}

int lobby_manager::notify(int fd)
{
    int valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    // closing
    if ((valread = read(fd, buffer, BUF_SIZE)) == 0) {
        return fd;
    }
    else {
        getpeername(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        buffer[valread] = '\0';
        std::cout << "Received \"\n" << buffer << "\" from " << inet_ntoa(address.sin_addr) << ":" 
                  << ntohs(address.sin_port) << std::endl;
        send(fd, buffer, strlen(buffer), 0);
        return -1;
    }
}
