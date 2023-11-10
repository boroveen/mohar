LDFLAGS = -lncursesw
CC = g++

all: mohar

mohar:
	$(CC) main.cpp -o main $(LDFLAGS)

clean:
	rm main
.PHONY: clean all
