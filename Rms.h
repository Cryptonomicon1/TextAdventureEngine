#ifndef RMS_H
#define RMS_H

#include"NPC.h"
#include"Tm.h"

#include<string>
using std::string;

#include<vector>
using std::vector;

#include<memory>
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::move;

class NPC;

class Rms {
public:
	Rms(string, string);
	~Rms();
	string gtNm();
	string gtBt();
	void ddTm(shared_ptr<Tm>);			//Add Item
	shared_ptr<Tm> gtTm(int);			//Get Item
	shared_ptr<Tm> tkTm(int);			//Take Item
	void ddNpc(shared_ptr<NPC>);		//Add Npc
	shared_ptr<NPC> gtNpc(int);			//Get Npc
	void stPth(shared_ptr<Rms>, int);	//Set Path
	shared_ptr<Rms> gtPth(int);			//Get Path

private:
	string nm;						//Name
	string bt;						//About
	vector<shared_ptr<Rms>> pths;	//North
	vector<shared_ptr<Tm>> tms;		//Items
	vector<shared_ptr<NPC>> npcs;	//Npcs
};
Rms::Rms(string n, string b) {
	nm = n;
	bt = b;
	pths.resize(4, nullptr);
};
Rms::~Rms() {
};
string Rms::gtNm() {
	return nm;
};
string Rms::gtBt() {
	return bt;
};
void Rms::stPth(shared_ptr<Rms> pth, int drctn) {
	pths[drctn] = pth;
};
shared_ptr<Rms> Rms::gtPth(int drctn) {
	if(drctn < pths.size()) {
		return pths[drctn];
	} else {
		return nullptr;
	};
};
void Rms::ddTm(shared_ptr<Tm> t) {
	tms.push_back(t);
};
shared_ptr<Tm> Rms::gtTm(int whch) {
	if(whch < tms.size()) {
		return tms[whch];
	} else {
		return nullptr;
	};
};
shared_ptr<Tm> Rms::tkTm(int whch) {
	if(whch < tms.size()) {
		shared_ptr<Tm> tm = tms[whch];
		tms.erase(tms.begin() + whch);
		return tm;
	} else return nullptr;
};
void Rms::ddNpc(shared_ptr<NPC> npc) {
	npcs.push_back(npc);
};
shared_ptr<NPC> Rms::gtNpc(int whch) {
	if(whch < npcs.size()) {
		return npcs[whch];
	} else {
		return nullptr;
	};
};
#endif
