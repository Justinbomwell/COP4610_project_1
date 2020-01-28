OBJS	= main.o executionProcessing.o
SOURCE	= main.c executionProcessing.c
HEADER	= executionProcessing.h
OUT	= shell
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99

executionProcessing.o: executionProcessing.c
	$(CC) $(FLAGS) executionProcessing.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)
