CC = gcc
CFLAGS = -ggdb -Wall

objects = gdt.o gdt_test.o
LIBS = -lcmocka
LIBDIR = /usr/local/lib


%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: ../%.c
	$(CC) $(CCFLAGS) -o $@ -c $<

gdt_test: $(objects)
	$(CC) $(CFLAGS) -o gdt_test $(objects) -L $(LIBDIR) $(LIBS)

clean:
	rm -rf *.o
	rm -rf gdt_test

run-all:
	./gdt_test