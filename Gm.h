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
	int mxmm_trggrs;

	enum {nmsk=0, rdr=1, sy=2, dd=3, ls=4, hv=0, cnt=1, hr=2};

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
	void ldRmLks();
};
Gm::Gm(string s) {
	cntr = 0;

	tpt_strng = "";
	
	io = make_shared<IO>("");
	bldMp();
	bldNpcs();
	bldTms();

	plyr = make_shared<NPC>();
	plyr->stRm(rms[2]);
	cmmnds = make_shared<Cmmnds>(io);
	mxmm_trggrs = gtMxmmTrggrs();
};
Gm::~Gm() {
};
void Gm::ply() {
	string npt;
	int npc_i = 0;
	int tm_i = 0;
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
		//tpt_strng = "";
		dRdrs(npt, tpt_strng);
		io->clrWndw();
		io->pdtTptWndw(tpt_strng);
		io->rfrshWndw();
		io->gtNpt();
	};
};
//I know this function has much repetition. However, I could either
//use 3 more nested for loops, or I can use a mathy solution
//involving ((i+1)%2)-1 to turn an i count into specific sequences.
//I figured the simple dumb repetative way is easy to maintain.
void Gm::bldMp() {
	vector<vector<string>> rooms = RdCsv("./Game/Rms1.csv");
	int fl_cnt = stoi( rooms[0][0].c_str() );
	rooms.erase( rooms.begin() );

	//Iterate over files if in 3D TODO add up/downstairs for 3d maps
	for(int i(1); i <= fl_cnt; i++) {

		//Iterate over y
		for(int j(0); j < rooms.size(); j++) {

			//Iterate over x
			for(int k(0); k < rooms[0].size(); k++) {
				if( rooms[j][k] != "" ) {
					if( fndRm( rooms[j][k] ) == nullptr
						&& rooms[j][k] != "" ) rms.push_back(
						make_shared<Rms>( rooms[j][k] ) );

					if( j != 0 && rooms[j-1][k] != "" ) {
						if( fndRm( rooms[j-1][k] ) == nullptr )
							rms.push_back(
							make_shared<Rms>( rooms[j-1][k] ));
						
						fndRm( rooms[j][k] )->stPth(
							fndRm( rooms[j-1][k] ), d.nrth );
							
						fndRm( rooms[j-1][k] )->stPth(
							fndRm(rooms[j][k] ), d.sth );
					};
					if( j != rooms.size() - 1 && rooms[j+1][k]!="") {
						if( fndRm( rooms[j+1][k] ) == nullptr )
							rms.push_back(
							make_shared<Rms>( rooms[j+1][k] ));

						fndRm( rooms[j][k] )->stPth(
							fndRm( rooms[j+1][k] ), d.sth );
							
						fndRm(rooms[j+1][k])->stPth(
							fndRm(rooms[j][k] ), d.nrth );
					};
					if( k != 0 && rooms[j][k-1] != "") {
						if( fndRm( rooms[j][k-1] ) == nullptr )
							rms.push_back(
							make_shared<Rms>( rooms[j][k-1] ));
						
						fndRm(rooms[j][k])->stPth(
							fndRm(rooms[j][k-1] ), d.wst );
							
						fndRm(rooms[j][k-1])->stPth(
							fndRm(rooms[j][k] ), d.st );
					};
					if(k != rooms[0].size()-1 && rooms[j][k+1]!="") {
						if( fndRm( rooms[j][k+1] ) == nullptr )
							rms.push_back(
							make_shared<Rms>( rooms[j][k+1] ));
						
						fndRm(rooms[j][k])->stPth(
							fndRm(rooms[j][k+1] ), d.st );
							
						fndRm(rooms[j][k+1])->stPth(
							fndRm(rooms[j][k] ), d.wst );
					};
				};
			};
		};
		if(i <= fl_cnt) {
			rooms = RdCsv("./Game/Rms" + to_string(i) + ".csv");
		};
	};
	ldRmLks();
};
//Load Room Looks
void Gm::ldRmLks() {
	vector<string> lks = Rd("./Game/Rms1.txt");
	int fl_cnt = stoi(lks[0]);
	lks.erase(lks.begin());

	for(int i(1); i <= fl_cnt; i++) {
		for(int j(0); j < lks.size(); j+=2) {
			fndRm( lks[j] )->stBt( lks[j+1] );
		};
		if(i <= fl_cnt) {
			lks = Rd("./Game/Rms" + to_string(i) + ".txt");
		};
	};
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
	return tpt;
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
	int j = 0;
	for(int i(0); i < rms.size(); i++) {
		j = 0;
		while(rms[i]->gtNpc(j) != nullptr) {
			if(rms[i]->gtNpc(j)->gtNm() == n) {
				return rms[i]->gtNpc(j);
			};
			j++;
		};
	};
	return nullptr;
};
void Gm::dRdrs(string npt, string tpt) {
	int j = 0;
	for(int i(0); i < rms.size(); i++) {
		j = 0;
		while(rms[i]->gtNpc(j) != nullptr) {
			dRdr(rms[i]->gtNpc(j), npt, tpt);
			j++;
		};
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
		
		} else if(trgs[i].rdr == ls) {
			cntr = mxmm_trggrs + 1;
			if(tpt_strng == " " || tpt_strng == "") {
				tpt_strng = "You lost the game. ";
			} else tpt_strng += " You lost the game. ";
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
