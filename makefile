CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = headers.h
OBJ = headers.o main.o normal_mode.o cmd_mode.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

FileExplorer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
