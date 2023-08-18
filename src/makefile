CROSS_TOOLS?=${CROSS_TOOL}

CC=${CROSS_TOOLS}gcc
C_FLAGS=-Wall -Werror -ansi -pedantic -g -std=c++0x -static
LIBS=-lstdc++ -lpthread -lm

ifneq (, $(findstring mingw, $(CC)))
	LIBS+=-lwsock32 -lws2_32
endif

all: clean server client

client: init_sockaddr.o myfun.o memtester.o 
	$(CC) -o memtester myfun.o init_sockaddr.o memtester.o $(C_FLAGS) $(LIBS)

myfun.o:
	$(CC) -o myfun.o -c myfun.cpp $(C_FLAGS) $(LIBS)

init_sockaddr.o:
	$(CC) -o init_sockaddr.o -c init_sockaddr.cpp $(C_FLAGS) $(LIBS)

memtester.o:
	$(CC) -o memtester.o -c memtester.cpp $(C_FLAGS) $(LIBS)

server: mem.o make_socket.o server.o
	$(CC) -o memtestd mem.o make_socket.o memtestd.o $(C_FLAGS) $(LIBS)

server.o:
	$(CC) -o memtestd.o -c memtestd.cpp $(C_FLAGS) $(LIBS)

make_socket.o:
	$(CC) -o make_socket.o -c make_socket.cpp $(C_FLAGS) $(LIBS)

mem.o:
	$(CC) -o mem.o -c mem.cpp $(C_FLAGS)


distclean: clean
	rm -f memtestd memtester
ifneq (, $(findstring mingw, $(CC)))
	rm -f memtestd.exe memtester.exe
endif

clean:
	rm -f *.o 

install:
	if [ ! -d $(DIST) ]; then \
		mkdir -p $(DIST); \
	fi
	cp memtestd memtester $(DIST)/