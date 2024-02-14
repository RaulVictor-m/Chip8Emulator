
build:
	g++ -I./dependencies/ src/Chip8.cpp src/Display.cpp src/Main.cpp -o chip8 -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17
