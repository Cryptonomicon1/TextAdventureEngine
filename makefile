all: Main

Main: Main.c++
	g++ Main.c++ -o Castle -lncurses -g
