# Chelcea Claudiu

# Seti[
CC=gcc
CFLAGS=-Wall -Wextra -g
LOAD=load_balancer
SERVER=server
CLIENT=client
LIST=linkedlist
NAME=loadApp

# Main call
build: app

app: main.o $(LOAD).o $(SERVER).o $(CLIENT).o $(LIST).o
	$(CC) $^ -o $(NAME)

main.o: main.c
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: $(SERVER).c $(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: $(LOAD).c $(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(CLIENT).o: $(CLIENT).c $(CLIENT).h
	$(CC) $(CFLAGS) $^ -c

$(LIST).o: $(LIST).c $(LIST).h
	$(CC) $(CFLAGS) $^ -c


clean:
	rm -f *.o $(NAME) *.h.gch

.PHONY: clean build
