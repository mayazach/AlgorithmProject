OBJS	= main.o
SOURCE	= main.cpp
HEADER	= curve.h
OUT 	= lsh
CC	= g++
FLAGS	= -c -g

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

clean:
	rm -f $(OBJS)
