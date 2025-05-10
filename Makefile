CC = cc
CFLAGS = -g -Wall
IFLAGS = -I/opt/homebrew/Cellar/sdl3/3.2.8/include
LDFLAGS = -L/opt/homebrew/Cellar/sdl3/3.2.8/lib/

build:
	${CC} ${CFLAGS} main.c ${IFLAGS} ${LDFLAGS} -lSDL3 -o main

run:
	./main

clean:
	rm main
