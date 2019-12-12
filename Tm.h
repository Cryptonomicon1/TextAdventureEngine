#ifndef TM_H
#define TM_H

#include<string>
using std::string;

class Tm {
public:
	Tm(string, string, string);
	~Tm();
	string gtNm();
	string gtBt();
	string gtQlfr();

private:
	string nm;				//Name
	string bt;				//About
	string qlfr;			//Qualifier
};
Tm::Tm(string n, string q, string b) {
	nm = n;
	qlfr = q;
	bt = b;
};
Tm::~Tm() {
};
string Tm::gtNm() {
	return nm;
};
string Tm::gtBt() {
	return bt;
};
string Tm::gtQlfr() {
	return qlfr;
};
#endif
