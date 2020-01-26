OBJS	= IOandPiping.o executionProcessing.o functions.o parser_help.o
SOURCE	= IOandPiping.c executionProcessing.c functions.c parser_help.c
HEADER	= executionProcessing.h
OUT	= shell
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

IOandPiping.o: IOandPiping.c
	$(CC) $(FLAGS) IOandPiping.c -std=c99

executionProcessing.o: executionProcessing.c
	$(CC) $(FLAGS) executionProcessing.c -std=c99

functions.o: functions.c
	$(CC) $(FLAGS) functions.c -std=c99

parser_help.o: parser_help.c
	$(CC) $(FLAGS) parser_help.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)

