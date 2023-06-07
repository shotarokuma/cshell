TARGET = cshell

CC = gcc
CFLAGS  = -g -Wall

all: ${TARGET}

${TARGET}: ${TARGET}.c ${TARGET}.h
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	-rm -f  $(TARGET) *.dmagit *.txt
