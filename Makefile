all:
	g++ snake.cpp snake_funcs.cpp  -o snake -I src/include/SDL2 -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf