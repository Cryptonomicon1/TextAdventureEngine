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
	vector<vector<string>> RdCsv(string);
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
vector<vector<string>> Nt::RdCsv(string fl) {
	string ln;
	vector<string> st;
	vector<vector<string>> tpt;
	ifstream f;
	f.open(fl);
	int i(0);
	while( getline(f, ln) ) {
		stringstream tmprry(ln);
		int j(0);
		st.clear();
		while( tmprry.good() ) {
			string sbstrng;
			getline(tmprry, sbstrng, ',');
			st.push_back(sbstrng);
			j++;
		};
		tpt.push_back(st);
	};
	return tpt;
};
#endif
