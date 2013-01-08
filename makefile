CFLAGS=-std=c++11
CC=g++
SDL_FLAGS=`sdl-config --libs --cflags` -lSDL_image -lSDL_ttf

all: AnimalFarm

AnimalFarm: main.o Game.o Game_Engine.o Sprite.o Static_Sprite.o Movable_Sprite.o Animal.o Weaponry.o Physics.o Time.o Map.o AI.o
	$(CC) main.o Game.o Game_Engine.o Sprite.o Static_Sprite.o Movable_Sprite.o Animal.o Weaponry.o Physics.o Time.o Map.o AI.o -o AnimalFarm $(SDL_FLAGS)

main.o: main.cpp
	\$(CC) -c $(CFLAGS) main.cpp $(SDL_FLAGS)

Game.o: Game.cpp
	\$(CC) -c $(CFLAGS) Game.cpp $(SDL_FLAGS)

Game_Engine.o: Game_Engine.cpp
	\$(CC) -c $(CFLAGS) Game_Engine.cpp $(SDL_FLAGS)

Sprite.o: Sprite.cpp
	\$(CC) -c $(CFLAGS) Sprite.cpp $(SDL_FLAGS)

Static_Sprite.o: Static_Sprite.cpp
	\$(CC) -c $(CFLAGS) Static_Sprite.cpp $(SDL_FLAGS)

Movable_Sprite.o: Movable_Sprite.cpp
	\$(CC) -c $(CFLAGS) Movable_Sprite.cpp $(SDL_FLAGS)

Animal.o: Animal.cpp
	\$(CC) -c $(CFLAGS) Animal.cpp $(SDL_FLAGS)

Weaponry.o: Weaponry.cpp
	\$(CC) -c $(CFLAGS) Weaponry.cpp $(SDL_FLAGS)	

Physics.o: Physics.cpp
	\$(CC) -c $(CFLAGS) Physics.cpp $(SDL_FLAGS)

Time.o: Time.cpp
	\$(CC) -c $(CFLAGS) Time.cpp $(SDL_FLAGS)

Map.o: Map.cpp
	\$(CC) -c $(CFLAGS) Map.cpp $(SDL_FLAGS)

AI.o: AI.cpp
	\$(CC) -c $(CFLAGS) AI.cpp $(SDL_FLAGS)

.PHONY clean:
	rm -f main.o Game.o Game_Engine.o Sprite.o Static_Sprite.o Movable_Sprite.o Animal.o Weaponry.o Physics.o Time.o Map.o AI.o
