compile:
	g++ -c main.cpp
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

run:
	./sfml-app
	
clean:
	rm main.o
	rm sfml-app
all:
	g++ -c main.cpp
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
	./sfml-app
