#ifndef IO_H
#define IO_H

#include<string>
using std::string;

#include<vector>
using std::vector;

//Don't forget to use the -lncurses flag when using g++
#include<ncurses.h>

class IO {
public:
	IO(string);
	~IO();

	void pdtPlcWndw(string);		//Update Place Window
	void pdtTmWndw(string);			//Update Item Window
	void pdtNpcWndw(string);		//Update NPC Window
	void pdtTptWndw(string);		//Update Output Window
	void pdtNptWndw(string);		//Update Input Window

	void rfrshWndw();				//Refresh Window
	void clrWndw();					//Clear Window
	string gtNpt();					//Get Input

private:
	//Variables
	WINDOW plc_wndw, tm_wndw, npc_wndw, tpt_wndw, npt_wndw;
	WINDOW plc_wndw_bx, tm_wndw_bx, npc_wndw_bx, tpt_wndw_bx, npt_wndw_bx;

	//Functions
	int fndBnnrStrt(int, int);
};
IO::IO(string str) {
	initscr();
	cbreak();
	noecho();
	int y_mxmm, x_mxmm;
	getmaxyx(stdscr, y_mxmm, x_mxmm);

	//Place Screen
	int tp = 0;
	int bttm = y_mxmm / 3;
	int lft = 0;
	int rght = x_mxmm;
	plc_wndw_bx = *newwin(bttm, rght, tp, lft);
	plc_wndw = *newwin(bttm-2, rght-2, tp+1, lft+1);

	//Item Screen
	tp = y_mxmm / 3;
	bttm = y_mxmm / 3;
	lft = 0;
	rght = x_mxmm / 2;
	tm_wndw_bx = *newwin(bttm, rght, tp, lft);
	tm_wndw = *newwin(bttm-2, rght-2, tp+1, lft+1);

	//NPC Screen
	tp = y_mxmm / 3;
	bttm = y_mxmm / 3;
	lft = x_mxmm / 2;
	rght = x_mxmm / 2;
	npc_wndw_bx = *newwin(bttm, rght, tp, lft);
	npc_wndw = *newwin(bttm-2, rght-2, tp+1, lft+1);

	//Output Screen
	tp = y_mxmm * 2 / 3;
	bttm = y_mxmm / 3 - 2;
	lft = 0;
	rght = x_mxmm;
	tpt_wndw_bx = *newwin(bttm, rght, tp, lft);
	tpt_wndw = *newwin(bttm-2, rght-2, tp+1, lft+1);
	
	//Input Screen
	tp = y_mxmm - 3;
	bttm = 3;
	lft = 0;
	rght = x_mxmm;
	npt_wndw_bx = *newwin(bttm, rght, tp, lft);
	npt_wndw = *newwin(bttm-2, rght-2, tp+1, lft+1);

	box(&plc_wndw_bx, 0, 0);
	box(&tm_wndw_bx, 0, 0);
	box(&npc_wndw_bx, 0, 0);
	box(&tpt_wndw_bx, 0, 0);
	box(&npt_wndw_bx, 0, 0);

	string bnnr = "Setting";
	int strt = fndBnnrStrt( x_mxmm, bnnr.size() );
	mvwprintw(&plc_wndw_bx, 0, strt, bnnr.c_str() );
	bnnr = "Items";
	strt = fndBnnrStrt( x_mxmm / 2, bnnr.size() );
	mvwprintw(&tm_wndw_bx, 0, strt, bnnr.c_str() );
	bnnr = "Characters";
	strt = fndBnnrStrt( x_mxmm / 2, bnnr.size() );
	mvwprintw(&npc_wndw_bx, 0, strt, bnnr.c_str() );
	bnnr = "Output";
	strt = fndBnnrStrt( x_mxmm, bnnr.size() );
	mvwprintw(&tpt_wndw_bx, 0, strt, bnnr.c_str() );
	bnnr = "Input";
	strt = fndBnnrStrt( x_mxmm, bnnr.size() );
	mvwprintw(&npt_wndw_bx, 0, strt, bnnr.c_str() );

	refresh();
	wrefresh(&plc_wndw_bx);
	wrefresh(&tm_wndw_bx);
	wrefresh(&npc_wndw_bx);
	wrefresh(&tpt_wndw_bx);
	wrefresh(&npt_wndw_bx);
};
IO::~IO() {
	delwin(&plc_wndw);
	delwin(&plc_wndw_bx);
	
	delwin(&tm_wndw);
	delwin(&tm_wndw_bx);
	
	delwin(&npc_wndw);
	delwin(&npc_wndw_bx);
	
	delwin(&tpt_wndw);
	delwin(&tpt_wndw_bx);
	
	delwin(&npt_wndw);
	delwin(&npt_wndw_bx);
	
	endwin();
};
void IO::pdtPlcWndw(string s) {
	wprintw(&plc_wndw, s.c_str());
};
void IO::pdtTmWndw(string s) {
	wprintw(&tm_wndw, s.c_str());
};
void IO::pdtNpcWndw(string s) {
	wprintw(&npc_wndw, s.c_str());
};
void IO::pdtTptWndw(string s) {
	wprintw(&tpt_wndw, s.c_str());
};
void IO::pdtNptWndw(string s) {
	wprintw(&npt_wndw, s.c_str());
};
void IO::rfrshWndw() {
	wrefresh(&plc_wndw);
	wrefresh(&tm_wndw);
	wrefresh(&npc_wndw);
	wrefresh(&tpt_wndw);
	wrefresh(&npt_wndw);
};
string IO::gtNpt() {
	int c;
	string ch;
	do {
		wclear(&npt_wndw);
		c = getchar();
		ch.push_back(c);
		wprintw(&npt_wndw, ch.c_str());
		wrefresh(&npt_wndw);
	} while(13 != c);
	
	ch.pop_back();
	return ch;
};
void IO::clrWndw() {
	wclear(&plc_wndw);
	wclear(&tm_wndw);
	wclear(&npc_wndw);
	wclear(&tpt_wndw);
	wclear(&npt_wndw);
	clear();
};
int IO::fndBnnrStrt(int wdth, int wrd_sz) {
	return (wdth-wrd_sz) / 2;
};
#endif
