OBJS	= main.o curveList.o distance.o
SOURCE	= main.cpp curveList.cpp distance.cpp
HEADER	= curve.h curveList.h distance.h
OUT 	= lsh
CC	= g++
FLAGS	= -c -g

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

curveList.o: curveList.cpp
	$(CC) $(FLAGS) curveList.cpp

distance.o: distance.cpp
	$(CC) $(FLAGS) distance.cpp
	
clean:
	rm -f $(OBJS)
