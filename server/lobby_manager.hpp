#ifndef LOBBY_MANAGER_H
#define LOBBY_MANAGER_H

#define BUF_SIZE 1024

class lobby_manager {
    private:
        char buffer[BUF_SIZE] = {'\0'};
    public:
        lobby_manager();
        int notify(int);
};

#endif // LOBBY_MANAGER_H 
