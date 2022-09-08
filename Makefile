CC = gcc
CFLAGS = -Wall -Wextra -Werror -ansi -pedantic
MAIN = smake
OBJS = main.o func.o pop.o trav.o free.o
DOTH = func.h pop.h trav.h free.h
all: $(MAIN)

$(MAIN) : $(OBJS) $(DOTH)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

main.o: main.c $(DOTH)
	$(CC) -gdwarf-2 -g3 -c main.c 

pop.o: pop.c $(DOTH)
	$(CC) -gdwarf-2 -g3 -c pop.c 

trav.o: trav.c $(DOTH)
	$(CC) -gdwarf-2 -g3 -c trav.c 

free.o: free.c $(DOTH)
	$(CC) -gdwarf-2 -g3 -c free.c

func.o: func.c $(DOTH)
	$(CC) -gdwarf-2 -g3 -c func.c

clean :
	rm *.o $(MAIN) core