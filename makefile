LIBS= -lGL -lGLU -lglut
CC= g++ -O2 -Wall -std=c++11 -pthread

OS: OS.o RGBpixmap.o
	$(CC) -o OS OS.o RGBpixmap.o

OS.o: OS.cpp RGBpixmap.o
	$(CC) -c OS.cpp

RGBpixmap.o: RGBpixmap.cpp RGBpixmap.h
	$(CC) -c RGBpixmap.cpp

clean:
	rm *.o OS *.gch
