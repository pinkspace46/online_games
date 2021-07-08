FLAGS = -Iinclude

all : build/client build/server

test :
	echo $(SERVER_SRC)
	echo $(CLIENT_SRC)

clean : 
	rm -rf build

SERVER_SRC = $(wildcard server/*.cpp)
SERVER_SRC += $(wildcard games/*.cpp)

CLIENT_SRC = $(wildcard client/*.cpp)

build/server : $(SERVER_SRC)
	mkdir -p $(@D)
	g++ $(FLAGS) -o $@ $(SERVER_SRC)

build/client : $(CLIENT_SRC)
	mkdir -p $(@D)
	g++ $(FLAGS) -o $@ $(CLIENT_SRC)
