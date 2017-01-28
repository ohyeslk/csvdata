CC=g++
CFLAGS=-std=c++11

all: csvdata

csvdata:  *.h *.cpp
		$(CC) $(CFLAGS) *.cpp -o csvdata

clean:
		rm -f csvdata csvdata.o
