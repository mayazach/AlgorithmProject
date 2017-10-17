OBJS	= main.o curveList.o
SOURCE	= main.cpp curveList.cpp
HEADER	= curve.h curveList.h
OUT 	= lsh
CC	= g++
FLAGS	= -c -g

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

curveList.o: curveList.cpp
	$(CC) $(FLAGS) curveList.cpp

clean:
	rm -f $(OBJS)
