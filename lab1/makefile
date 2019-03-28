CC := gcc

CFLAGS := -std=c99 -Wall -Werror -pedantic


app.exe : main.o

	$(CC) -o $@ $^
%.o : %.c
	$(CC) $(CFLAGS) -g3 -ggdb -c $<

clean:

	rm *.o *.exe