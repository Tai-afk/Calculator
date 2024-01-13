CC = g++
CFLAGS = -Wall

calc: calc.cpp
	$(CC) calc.cpp $(CFLAGS) -o calc

clean:
	rm *.o