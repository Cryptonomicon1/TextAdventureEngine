#ifndef MRMR3_H
#define MRMR3_H

//This is a basic c++ implimentation of the murmur3 hash.
//I chose this hash, because It is a relatively fast and
//spread out hash for hash tables.

#include<string>
using std::string;

#include<vector>
using std::vector;

#include<cstdint>
using std::uint8_t;
using std::uint32_t;

#include<iostream>
using std::cout;
using std::endl;

class Mrmr3 {
public:
	Mrmr3();
	~Mrmr3();
	uint32_t Hsh(string, uint32_t);
private:
	//Variables and Constants
	const static uint32_t c1 = 0xcc9e2d51;
	const static uint32_t c2 = 0x1b873593;
	const static uint32_t r1 = 15;
	const static uint32_t r2 = 13;
	const static uint32_t m = 5;
	const static uint32_t n = 0xe6546b64;

	//Functions
	uint32_t Rtt(uint32_t, int);			//Rotate
	uint32_t FnlMx(uint32_t);				//Final Mix
	uint32_t NdnSwp(string);				//Endian Swap
	uint32_t GtBlck(string, int);			//Get Block
};
Mrmr3::Mrmr3() {
};
Mrmr3::~Mrmr3() {
};
uint32_t Mrmr3::Hsh(string ky, uint32_t sd) {
	uint32_t hsh = sd;						//Seed <- Hash
	int ky_lngth = ky.size();				//Key Length
	uint32_t ntgr_ky(0);
	if(ky.size() > 3) {
		int i = ky.size() / 4;				//Iterator <- Key_Size
		do {
			ntgr_ky = GtBlck(ky, 4);
			ky.erase(0,4);
			ntgr_ky *= c1;
			ntgr_ky = Rtt(ntgr_ky, r1);
			ntgr_ky *= c2;
			hsh ^= ntgr_ky;
			hsh = Rtt(hsh, r2);
			hsh = (hsh * m) + n;
		} while(--i);
	};
	ntgr_ky = 0;
	uint32_t tl = 0;			//Tail
	int tst(1);
	for(int i(ky_lngth & 3); i >= 0; i--) {
		
	};
	if(*(char *)&tst != 1) {
		tl = NdnSwp(ky);
	};
	switch(ky_lngth & 3) {
		case 3 : ntgr_ky ^= GtBlck(ky, 3);
		case 2 : ntgr_ky ^= GtBlck(ky, 2);
		case 1 : ntgr_ky ^= GtBlck(ky, 1);

		ntgr_ky *= c1;
		ntgr_ky = Rtt(ntgr_ky, r1);
		ntgr_ky *= c2;
		hsh ^= ntgr_ky;
	};
	hsh ^= ky_lngth;
	return FnlMx(hsh);
};
uint32_t Mrmr3::Rtt(uint32_t x, int r) {
	return (x << r) | (x >> (32-r));
};
uint32_t Mrmr3::FnlMx(uint32_t hsh) {
	hsh ^= hsh >> 16;
	hsh *= 0x85ebca6b;
	hsh ^= hsh >> 13;
	hsh *= 0xc2b2ae35;
	hsh ^= hsh >> 16;
	return hsh;
};
uint32_t Mrmr3::NdnSwp(string tl) {
	uint32_t tpt = 0;			//Output
	int i = tl.size();
	do {
		tpt <<= 8;
		tpt |= int(*tl.substr(i-1,1).c_str());
		tl.pop_back();
	} while(--i);
	return tpt;
};
uint32_t Mrmr3::GtBlck(string strng, int blcks) {
	uint32_t tpt;				//Output
	for(int i(blcks-1); i >= 0; i--) {
 		tpt = int(*strng.substr(0,1).c_str()) << (i*8);
		strng.erase(0,1);
	};
	return tpt;
};
#endif
