CC = gcc
CFLAGS = -Iinclude

SRC = src/learn_makefile.c src/printfunc.c
OBJ = obj/learn_makefile.o obj/printfunc.o
TARGET = task1

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/*.o $(TARGET)
