#ifndef GM_H
#define GM_H

#include"Drctns.h"
#include"IO.h"
#include"NPC.h"
#include"Tm.h"
#include"Rms.h"
#include"Cmmnds.h"
#include"Nt.h"

#include<memory>
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;
using std::make_shared;

#include<vector>
using std::vector;

#include<string>
using std::string;
using std::to_string;
using std::stoi;
/*
struct trggr {
	//Conditions
	int thrsh;			//Threshhold
	string cnd_mssg;	//Condition Message
	int cnd;			//Condition

	//Orders
	bool msk;			//Mask
	string rdr_mssg;	//Order Message
	int rdr;			//Order
};
*/
class Gm : private Nt {
public:
	Gm(string);
	~Gm();
	void ply();

private:
	//Variables
	shared_ptr<NPC> plyr;
	vector<shared_ptr<Rms>> rms;
	shared_ptr<IO> io;
	shared_ptr<Cmmnds> cmmnds;
	int cntr;
	string tpt_strng;

	enum {nmsk=0, rdr=1, sy=2, dd=3, hv=0, cnt=1, hr=2};

	int hll, gt, alch, blck, crt;

	//Functions
	void bldMp();
	void bldNpcs();
	void bldTms();
	int gtTrggrCnt();
	int gtMxmmTrggrs();
	int gtRmNdx(string);
	shared_ptr<Rms> fndRm(string);
	shared_ptr<NPC> fndNpc(string);
	void dRdrs(string, string);
	void dRdr(shared_ptr<NPC>, string, string);
	string fltrStrng(string, string);
};
Gm::Gm(string s) {
	crt = 0;
	hll = 1;
	blck = 2;
	gt = 4;
	alch = 3;

	cntr = 1;

	tpt_strng = "";
	
	io = make_shared<IO>("");
	bldMp();
	bldNpcs();
	bldTms();

	plyr = make_shared<NPC>();
	plyr->stRm(rms[0]);
	cmmnds = make_shared<Cmmnds>(io);
};
Gm::~Gm() {
};
void Gm::ply() {
	string npt;
	int npc_i = 0;
	int tm_i = 0;
	int mxmm_trggrs = gtMxmmTrggrs();
	do {
		io->clrWndw();
		io->pdtPlcWndw(plyr->gtRm()->gtNm() + "\n");
		io->pdtPlcWndw(plyr->gtRm()->gtBt() + "\n\n");
		for(int i(0); i < 4; i++) {
			if(plyr->gtRm()->gtPth(i) != nullptr) {
				io->pdtPlcWndw(d.n[i] + " is the ");
				io->pdtPlcWndw( plyr->gtRm()->gtPth(i)->gtNm() );
				io->pdtPlcWndw("   ");
			};
			if(i == 1) io->pdtPlcWndw("\n");
		};
		tm_i = 0;
		while( plyr->gtRm()->gtTm(tm_i) != nullptr ) {
			io->pdtTmWndw( plyr->gtRm()->gtTm(tm_i)->gtQlfr() + " ");
			io->pdtTmWndw( plyr->gtRm()->gtTm(tm_i)->gtNm() );
			io->pdtTmWndw("\n");
			tm_i++;
		};
		tm_i = 0;
		while( plyr->gtTm(tm_i) != nullptr ) {
			io->pdtTmWndw( "**" + plyr->gtTm(tm_i)->gtQlfr() + " " );
			io->pdtTmWndw( plyr->gtTm(tm_i)->gtNm() + "**\n" );
			tm_i++;
		};
		npc_i = 0;
		while( plyr->gtRm()->gtNpc(npc_i) != nullptr ) {
			io->pdtNpcWndw( plyr->gtRm()->gtNpc(npc_i)->gtClss() );
			io->pdtNpcWndw( " " );
			io->pdtNpcWndw( plyr->gtRm()->gtNpc(npc_i)->gtNm() );
			io->pdtNpcWndw( "\n" );
			npc_i++;
		};
		io->pdtTptWndw(tpt_strng);
		io->rfrshWndw();
		tpt_strng = "";
		npt = io->gtNpt();
		tpt_strng = cmmnds->D(plyr, npt, gtTrggrCnt());
		dRdrs(npt, tpt_strng);
	
	} while(npt != "q" && npt != "quit" &&
			npt != "Quit" && gtTrggrCnt() < mxmm_trggrs);

	if(gtTrggrCnt() >= mxmm_trggrs) {
		tpt_strng = "";
		dRdrs(npt, tpt_strng);
		io->clrWndw();
		io->pdtTptWndw(tpt_strng);
		io->rfrshWndw();
		io->gtNpt();
	};
};
void Gm::bldMp() {
	rms.push_back( make_shared<Rms>("Courtyard", "Vendors and tradesmen hustle and bustle in the courtyard around you.") );

	rms.push_back( make_shared<Rms>("Grand Hall", "Various people eat at long tables for a high price.") );
	
	rms.push_back( make_shared<Rms>("Blacksmith's Shop", "The Blacksmith ignores you as he tends to his forge.") );
	
	rms.push_back( make_shared<Rms>("Alchemist's Shop", "The Alchemist mumbles about making gold as he plays with chemicals. He looks high.") );
	
	rms.push_back( make_shared<Rms>("Front Gate", "Guards sleep on duty as a thief takes a sword from a nearby rack.") );

	//Courtyard to Grand Hall
	rms[crt]->stPth(rms[hll], d.nrth);
	rms[hll]->stPth(rms[crt], d.sth);

	//Courtyard to Blacksmith's Shop
	rms[crt]->stPth(rms[blck], d.st);
	rms[blck]->stPth(rms[crt], d.wst);
	
	//Courtyard to Front Gate
	rms[crt]->stPth(rms[gt], d.sth);
	rms[gt]->stPth(rms[crt], d.nrth);
	
	//Courtyard to Alchemist's Shop
	rms[crt]->stPth(rms[alch], d.wst);
	rms[alch]->stPth(rms[crt], d.st);
};
void Gm::bldNpcs() {
	vector<string> dt = Rd("./Game/Npcs1.txt");
	vector<string> npt;
	shared_ptr<NPC> npc;
	int nd = stoi( dt.front() );
	dt.erase(dt.begin());

	//Triple Loop
	//1st Loop decides file to load
	//2nd Loop ensures that we read all the txt
	//3rd Loop reads the file txt between the "Npc" tags
	//	and uses them to make npcs
	for(int i(1); i <= nd; i++) {
		while(dt.size() > 0) {
			while(dt.size() > 0 && dt[0].substr(0,3) != "Npc") {
				npt.push_back( dt[0] );
				dt.erase(dt.begin());
			};
			if(dt.size() > 0) dt.erase(dt.begin()) ;
			if(npt.size() > 0) {
				npc = make_shared<NPC>(npt);
				fndRm(npt[0])->ddNpc(npc);
				npc->stRm( fndRm(npt[0]) );
				npt.clear();
			};
		};
		if(i+1 <= nd) {
			dt.clear();
			dt = Rd("./Game/Npcs" + to_string(i+1) + ".txt");
		};
	};
};
void Gm::bldTms() {
	vector<string> dt = Rd("./Game/Items1.txt");
	vector<string> npt;
	shared_ptr<Tm> tm;
	shared_ptr<Rms> rm;
	shared_ptr<NPC> npc;
	int nd = stoi( dt.front() );
	dt.erase(dt.begin());

	//Triple Loop
	//1st Loop decides file to load
	//2nd Loop ensures that we read all the txt
	//3rd Loop reads the file txt between the "Npc" tags
	//	and uses them to make npcs
	for(int i(1); i <= nd; i++) {
		while(dt.size() > 0) {
			while(dt.size() > 0 && dt[0].substr(0,4) != "Item") {
				npt.push_back( dt[0] );
				dt.erase(dt.begin());
			};
			if(dt.size() > 0) dt.erase(dt.begin());
			if(npt.size() > 0) {
				npc = fndNpc(npt[0]);
				rm = fndRm(npt[0]);
			};
			if(npt.size() > 0 && rm != nullptr) {
				rm->ddTm(make_shared<Tm>(npt[1], npt[2], npt[3]));
				npt.clear();
			} else if(npt.size() > 0 && npc != nullptr) {
				npc->ddTm(make_shared<Tm>(npt[1], npt[2], npt[3]));
				npt.clear();
			};
		};
		if(i+1 <= nd) {
			dt.clear();
			dt = Rd("./Game/Items" + to_string(i+1) + ".txt");
		};
	};
};
int Gm::gtTrggrCnt() {
	return cntr;
};
int Gm::gtMxmmTrggrs() {
	int j, tpt = 0;
	for(int i(0); i < rms.size(); i++) {
		j = 0;
		while(rms[i]->gtNpc(j) != nullptr ) {
			tpt += rms[i]->gtNpc(j)->gtMxmmTrggrCnt();
			j++;
		};
	};
	return tpt + 1;
};
int Gm::gtRmNdx(string nm) {
	int i = 0;
	while(rms[i] != nullptr) {
		if(rms[i]->gtNm() == nm) return i;
		i++;
	};
	return -1;
};
shared_ptr<Rms> Gm::fndRm(string nm) {
	int i = 0;
	while(rms.size() > i && rms[i] != nullptr) {
		if(rms[i]->gtNm() == nm) return rms[i];
		i++;
	};
	return nullptr;
};
shared_ptr<NPC> Gm::fndNpc(string n) {
	int i = 0, j = 0;
	while(rms[i] != nullptr) {
		j = 0;
		while(rms[i]->gtNpc(j) != nullptr) {
			if(rms[i]->gtNpc(j)->gtNm() == n) {
				return rms[i]->gtNpc(j);
			};
			j++;
		};
		i++;
	};
	return nullptr;
};
void Gm::dRdrs(string npt, string tpt) {
	int i = 0, j = 0;
	while(rms[i] != nullptr) {
		j = 0;
		while(rms[i]->gtNpc(j) != nullptr) {
			dRdr(rms[i]->gtNpc(j), npt, tpt);
			j++;
		};
		i++;
	};
};
void Gm::dRdr(shared_ptr<NPC> n, string npt, string tpt) {
	npt = fltrStrng( cmmnds->lwrCs( n->gtNm() ), npt);
	vector<trggr> trgs = n->chckCndtn(npt, tpt, gtTrggrCnt() );

	for(int i(0); i < trgs.size(); i++) {

		//Flip Mask Bit?
		if(trgs[i].rdr == nmsk) {
			trgs[i].msk = true;
			n->stTrgrMsk( trgs[i] );

		//Execute Command?
		} else if(trgs[i].rdr == rdr) {
			cmmnds->D( n, trgs[i].rdr_mssg, gtTrggrCnt() );
		
		} else if(trgs[i].rdr == sy) {
			if(tpt_strng == " " || tpt_strng.size() == 0) {
				tpt_strng = trgs[i].rdr_mssg;
			} else tpt_strng += trgs[i].rdr_mssg;
		
		} else if(trgs[i].rdr == dd) {
			cntr += stoi( trgs[i].rdr_mssg );
		};
	};
};
string Gm::fltrStrng(string nm, string s) {
	if(s.find("to") != string::npos && s.substr(0, 3) == "say") {
		int fnsh = s.find("to");
		if( nm == s.substr(fnsh + 3) ) {
			return s.substr(4, fnsh - 5);
		};
	};
	return " ";
};
#endif
