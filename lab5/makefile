CC := gcc

CFLAGS := -std=c99 -Wall -Werror -pedantic

OBJS := main.o in_out.o func.o

app.exe : $(OBJS)
	$(CC) -o $@ $^

main.o : main.c defines.h in_out.h func.h
	$(CC) $(CFLAGS) -g3 -ggdb -c main.c

func.o : func.c defines.h in_out.h func.h
	$(CC) $(CFLAGS) -g3 -ggdb -c func.c

in_out.o : in_out.c defines.h in_out.h
	$(CC) $(CFLAGS) -g3 -ggdb -c in_out.c


clean:

	rm *.o *.exe