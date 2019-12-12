#ifndef DRCTNS_h
#define DRCTNS_h

#include<vector>
using std::vector;

#include<string>
using std::string;

static struct directions {
	int nrth = 0;
	int st = 1;
	int sth = 2;
	int wst = 3;

	string n[4] = {"North", "East", "South", "West"};
} d;

#endif
