# Chelcea Claudiu

# Seti[
CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11 -pedantic -pedantic-errors
LOAD=load_balancer
SERVER=server
CLIENT=client
NAME=tema2

# Main call
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

.PHONY: clean build
