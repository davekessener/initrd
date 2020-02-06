CC=g++
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
DEP=$(wildcard *.h)
CFLAGS=-Wall -Werror -ggdb -O0
LDFLAGS=
TARGET=mkinitrd

.PHONY: all clean

%.o: %.cc $(DEP)
	$(CC) -o $@ -c $< $(CFLAGS)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

