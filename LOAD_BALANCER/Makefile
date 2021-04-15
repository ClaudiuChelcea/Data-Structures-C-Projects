CC=gcc
CFLAGS=-Wall -Wextra
LOAD=load_balancer
SERVER=server
CLIENT=client
NAME=loadApp

.PHONY: build clean

build: app

app: main.o $(LOAD).o $(SERVER).o $(CLIENT).o
	$(CC) $^ -o $(NAME)

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(CLIENT).o: $(CLIENT).c $(CLIENT).h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o $(NAME) *.h.gch
