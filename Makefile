CC = g++
CFLAGS = -Wall

calc: calc.cpp
	$(CC) -g calc.cpp $(CFLAGS) -o calc

clean:
	rm *.o