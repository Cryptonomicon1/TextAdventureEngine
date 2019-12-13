#ifndef CMMNDS_H
#define CMMNDS_H

#include"NPC.h"
#include"IO.h"
#include"Drctns.h"

#include<memory>
using std::shared_ptr;

#include<string>
using std::string;

class Cmmnds {
public:
	Cmmnds(shared_ptr<IO>);
	~Cmmnds();
	string D(shared_ptr<NPC>, string, int);		//Do Command
	string lwrCs(string);			//Lower The Case

private:
	//Variables
	//shared_ptr<NPC> plyr;
	shared_ptr<IO> io;
	string n_pth;					//No Path
	string n_nm;					//No Character Name
	string n_tm;					//No Item

	//Functions
	string dltPncttn(string);		//Delete Punctuation
	shared_ptr<NPC> fndNpc(shared_ptr<NPC>, string);	//Find Npc
};
Cmmnds::Cmmnds(shared_ptr<IO> i) {
	io = i;
	n_pth = "There is no path in that direction";
	n_nm = "No one is here with that name.";
	n_tm = "No item is here with that name.";
};
Cmmnds::~Cmmnds() {
};
string Cmmnds::D(shared_ptr<NPC> p, string cmmnd, int trggr) {
	shared_ptr<NPC> plyr = p;
	cmmnd = lwrCs(cmmnd);
	string fnl_tpt = " ";		//Final Output

	if(cmmnd == "" || cmmnd == " ") {
		fnl_tpt = " ";
	} else if(cmmnd.substr(0, 2) == "go") {
		if(cmmnd.size() >= 4) {
			switch( cmmnd.at(3) ) {
				case 'n' :
					if(plyr->gtRm()->gtPth(d.nrth) != nullptr) {
						plyr->stRm( plyr->gtRm()->gtPth(d.nrth) );
					} else fnl_tpt = n_pth;
					break;
				case 'e' :
					if(plyr->gtRm()->gtPth(d.st) != nullptr) {
						plyr->stRm( plyr->gtRm()->gtPth(d.st) );
					} else fnl_tpt = n_pth;
					break;
				case 's' :
					if(plyr->gtRm()->gtPth(d.sth) != nullptr) {
						plyr->stRm( plyr->gtRm()->gtPth(d.sth) );
					} else fnl_tpt = n_pth;
					break;
				case 'w' :
					if(plyr->gtRm()->gtPth(d.wst) != nullptr) {
					plyr->stRm( plyr->gtRm()->gtPth(d.wst) );
					} else fnl_tpt = n_pth;
					break;
				default :
					fnl_tpt = n_pth;
			}
		} else fnl_tpt = "";
	} else if(cmmnd.substr(0, 3) == "say") {
		int t = cmmnd.find("to");
		string tpt = cmmnd.substr( t + 3 );
		shared_ptr<NPC> npc = fndNpc(plyr, tpt);
		if(npc != nullptr) {
			fnl_tpt = npc->gtRspns( cmmnd.substr(4, t - 5), trggr );
		} else if(cmmnd.substr(t) == "to myself") {
			fnl_tpt = cmmnd.substr(4, t - 5);
		} else fnl_tpt = n_nm;
	} else if(cmmnd.substr(0, 5) == "study") {
		int i = 0;
		shared_ptr<Rms> rm = plyr->gtRm();
		while(rm->gtTm(i) != nullptr) {
			if(lwrCs( rm->gtTm(i)->gtNm() ) == cmmnd.substr(6)) {
				fnl_tpt = rm->gtTm(i)->gtBt();
				break;
			};
			i++;
		};
		int i2 = 0;
		while(plyr->gtTm(i2) != nullptr) {
			if(lwrCs( plyr->gtTm(i2)->gtNm() ) == cmmnd.substr(6)) {
				fnl_tpt = plyr->gtTm(i2)->gtBt();
				break;
			};
			i2++;
		};
		if(	rm->gtTm(i) == nullptr &&
			plyr->gtTm(i2) == nullptr) fnl_tpt = n_tm;
	} else if(cmmnd.substr(0, 4) == "take") {
		int i = 0;
		bool sccs = false;		//Success
		shared_ptr<Rms> rm = plyr->gtRm();
		while(rm->gtTm(i) != nullptr) {
			if(lwrCs( rm->gtTm(i)->gtNm() ) == cmmnd.substr(5)) {
				plyr->ddTm(rm->tkTm(i));
				fnl_tpt = " ";
				sccs = true;
				break;
			};
			i++;
		};
		if(!sccs) fnl_tpt = n_tm;
	} else if(cmmnd.substr(0, 4) == "drop") {
		int i = 0;
		bool sccs = false;		//Success
		shared_ptr<Rms> rm = plyr->gtRm();
		while(plyr->gtTm(i) != nullptr) {
			if(lwrCs( plyr->gtTm(i)->gtNm() ) == cmmnd.substr(5)) {
				rm->ddTm(plyr->tkTm(i));
				fnl_tpt = " ";
				sccs = true;
				break;
			};
			i++;
		};
		if(!sccs) fnl_tpt = n_tm;
	} else if(cmmnd.substr(0, 4) == "give") {
		int t = cmmnd.find("to");
		string tpt = cmmnd.substr( t + 3 );
		shared_ptr<NPC> npc = fndNpc(plyr, tpt);
		if(npc != nullptr) {
			int j = 0;
			while(	plyr->gtTm(j) != nullptr && lwrCs( plyr->gtTm(j)->gtNm() ) != cmmnd.substr(5, t-6) ) {
				j++;
			};
			if( plyr->gtTm(j) != nullptr && lwrCs( plyr->gtTm(j)->gtNm() ) == cmmnd.substr(5, t-6) ) {
				npc->ddTm(plyr->tkTm(j));
			};
		} else {
			fnl_tpt = n_nm;
		};
	} else if(false) {
	} else if(false) {
	} else if(false) {
	} else fnl_tpt = "Please Try Again: Bad Command";

	if(fnl_tpt == "") fnl_tpt = "Please Try Again: Bad Command";
	return fnl_tpt;
};
string Cmmnds::lwrCs(string s) {
	for(int i(0); i < s.size(); i++) {
		if('A' <= s.at(i) && s.at(i) <= 'Z') {
			s.at(i) = s.at(i) + 32;
		};
	};
	return s;
};
string Cmmnds::dltPncttn(string s) {
	for(int i(0); i < s.size(); i++) {
		if(s[i] == '.' || s[i] == ',' || s[i] == '!' || s[i] =='?') {
			s.erase(i, 1);
			i--;
		};
	};
	return s;
};
shared_ptr<NPC> Cmmnds::fndNpc(shared_ptr<NPC> plyr, string nm) {
	int i = 0;
	while(plyr->gtRm()->gtNpc(i) != nullptr) {
		if( lwrCs( plyr->gtRm()->gtNpc(i)->gtNm() ) == nm) {
			return plyr->gtRm()->gtNpc(i);
		};
		i++;
	};
	return nullptr;
};
#endif
