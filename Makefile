# Created by Kenneth Lee (klee33@uw.edu)

CC = gcc
CFLAGS = -Wall -std=c17
OBJFILES = wordle.o words.o
TARGET = wordle

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET) *~