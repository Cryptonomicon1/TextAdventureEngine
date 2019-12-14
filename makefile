all: Main

Main: Main.c++
	g++ Main.c++ -o TxtDvntr -lncurses -g
