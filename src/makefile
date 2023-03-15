CROSS_TOOLS?=${CROSS_TOOL}

CC=${CROSS_TOOLS}gcc
C_FLAGS=-Wall -Werror -ansi -pedantic -g -std=c++0x
LDFLAG=-lpthread -lm
LDFLAG+=-lwsock32

all: clean server client

client: init_sockaddr.o memtester.o
	$(CC) -o memtester init_sockaddr.o memtester.o $(LDFLAG)

init_sockaddr.o:
	$(CC) -o init_sockaddr.o -c init_sockaddr.cpp $(C_FLAGS)

memtester.o:
	$(CC) -o memtester.o -c memtester.cpp $(C_FLAGS)

server: mem.o make_socket.o server.o
	$(CC) -o memtestd mem.o make_socket.o memtestd.o $(LDFLAG)

server.o:
	$(CC) -o memtestd.o -c memtestd.cpp $(C_FLAGS)

make_socket.o:
	$(CC) -o make_socket.o -c make_socket.cpp $(C_FLAGS)

mem.o:
	$(CC) -o mem.o -c mem.cpp $(C_FLAGS)

clean:
	rm -f *.o memtestd memtester

install:
	if [ ! -d $(DIST) ]; then \
		mkdir -p $(DIST); \
	fi
	cp memtestd memtester $(DIST)/