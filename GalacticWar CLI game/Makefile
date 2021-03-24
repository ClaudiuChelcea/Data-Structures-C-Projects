# Chelcea Claudiu Marian

# Setup
CC=clang
FLAGS=-Wall -Wextra -g
TARGETS=TheGalacticWar.o GalacticFunctions.o Galaxy.o
NAME=play

# Compile files individually and then link them
build: TheGalacticWar GalacticFunctions Galaxy Link

TheGalacticWar: TheGalacticWar.c
	$(CC) -c TheGalacticWar.c $(FLAGS)

GalacticFunctions: GalacticFunctions.c
	$(CC) -c GalacticFunctions.c $(FLAGS)

Galaxy: Galaxy.c
	$(CC) -c Galaxy.c $(FLAGS)

Link: $(TARGETS)
	$(CC) -o $(NAME) $(TARGETS)

clean:
	rm -rf $(TARGETS)
	rm -rf $(NAME)

.PHONY: clean
