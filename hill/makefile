CC = gcc
CFLAGS =
TARGET = dt.c
OBJS = main.o tree.o
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
main.o: main.c tree.h
	$(CC) $(CFLAGS) -c main.c
tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c
clean:
	/bin/rm -f *.o $(TARGET)
