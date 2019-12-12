#ifndef NT_H
#define NT_H

#include<string>
using std::string;

#include<vector>
using std::vector;

#include<iostream>
using std::iostream;

#include<fstream>
using std::ifstream;

#include<sstream>
using std::stringstream;

class Nt {
public:
	Nt();
	~Nt();
	vector<string> Rd(string);
private:
};
Nt::Nt() {
};
Nt::~Nt() {
};
vector<string> Nt::Rd(string fl) {
	string ln;
	vector<string> tpt;
	ifstream dt;
	dt.open(fl);
	while(getline(dt, ln)) {
		if(ln.size() > 0) tpt.push_back(ln);
	};
	return tpt;
};
#endif
