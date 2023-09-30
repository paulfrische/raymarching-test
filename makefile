LIBS = -lGL -lGLEW -lglfw
CFLAGS = -o build/run -Ofast
SRC = $(wildcard ./src/*.cpp)
CC = clang++

all:
	mkdir -p build
	$(CC) $(SRC) $(CFLAGS) $(LIBS)

test: all
	./build/run
