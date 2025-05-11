a.out: main.cpp pca9685.o
	g++ main.cpp pca9685.o -llgpio -lSDL2

pca9685.o: pca9685.h pca9685.cpp
	g++ -c pca9685.cpp
