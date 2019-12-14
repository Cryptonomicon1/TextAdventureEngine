#ifndef NPC_H
#define NPC_H

#include"Rms.h"
#include"HshTbl.h"
#include"Nt.h"
#include"Tm.h"

#include<string>
using std::string;
using std::stoi;

#include<memory>
using std::shared_ptr;
using std::move;

struct trggr {
	//Conditions
	int thrsh; 			//threshhold
	string cnd_mssg;	//have item
	int cnd;			//condition
	
	//Orders
	bool msk; 			//mask
	string rdr_mssg;	//order message
	int rdr;			//order
};
class Rms;

class NPC : private Nt {
public:
	NPC();
	NPC(vector<string>);
	~NPC();
	string gtNm();
	string gtClss();
	void stRm(shared_ptr<Rms>);
	shared_ptr<Rms> gtRm();
	string gtRspns(string, int);
	void ddTm(shared_ptr<Tm>);
	shared_ptr<Tm> gtTm(int);
	shared_ptr<Tm> tkTm(int);
	int gtTrggrCnt();
	int gtMxmmTrggrCnt();
	vector<trggr> chckCndtn(string, string, int);
	void stTrgrMsk(trggr);

private:
	//Variables
	string nm;
	string clss;
	shared_ptr<Rms> rm;
	vector<shared_ptr<Tm>> tms;
	string stndrd_rspns;
	string nt_yt;
	HshTbl<string> hshtbl;
	vector<int> trggr_thrsh;
	vector<string> trggr_txt;
	vector<bool> trggr_stt;
	string rm_nm;
	vector<trggr> trgs;
	int mxmm_trggrs;

	enum {nmsk=0, rdr=1, sy=2, dd=3, ls=4, hv=0, cnt=1, hr=2, sd=3};

	//Functions
	void ldChts(vector<string>);					//Load Chats
	vector<string> ldTrggr(vector<string>, int);	//Load Triggers
	void ldCndtn(string);							//Load Condition

};
NPC::NPC(vector<string> n) {
	mxmm_trggrs = 0;
	hshtbl = HshTbl<string>();

	ldChts(n);
	stndrd_rspns = "What did you say? I don't know what you are talking about?";
	nt_yt = "Sorry, I cannot tell you that yet.";
};
NPC::NPC() {
	mxmm_trggrs = 0;
	hshtbl = HshTbl<string>();
	stndrd_rspns = "What did you say? I don't know what you are talking about?";
	nt_yt = "Sorry, I cannot tell you that yet.";
};
NPC::~NPC() {
};
string NPC::gtNm() {
	return nm;
};
string NPC::gtClss() {
	return clss;
};
void NPC::stRm(shared_ptr<Rms> r) {
	rm = r;
	rm_nm = rm->gtNm();
};
shared_ptr<Rms> NPC::gtRm() {
	return rm;
};
void NPC::ddTm(shared_ptr<Tm> tm) {
	tms.push_back(tm);
};
shared_ptr<Tm> NPC::gtTm(int ndx) {
	if(ndx < tms.size()) {
		return tms[ndx];
	} else return nullptr;
};
shared_ptr<Tm> NPC::tkTm(int ndx) {
	if(ndx < tms.size()) {
		shared_ptr<Tm> tm = tms[ndx];
		tms.erase(tms.begin() + ndx);
		return tm;
	} else return nullptr;
};
void NPC::ldChts(vector<string> chts) {
	rm_nm = chts[0];
	chts.erase(chts.begin());
	clss = chts[0];
	chts.erase(chts.begin());
	nm = chts[0];
	chts.erase(chts.begin());

	for(int i(0); i < chts.size(); i+=2) {
		if(chts[i].substr(0, 7) == "trigger"){
			chts = ldTrggr(chts, i);
			i-=2;
		};
		if(i >= 0) hshtbl.Gv(chts[i], chts[i+1]);
	};
};
//Get Response from NPC
string NPC::gtRspns(string npt, int trg) {
	string tpt = hshtbl.Tk(npt);	//Output
	bool lt_tpt = false;			//Let output happen
	bool n_msk = true;				//Response is in mask

	for(int i(0); i < trgs.size(); i++) {
		if(trgs[i].cnd == hr && trgs[i].cnd_mssg == npt) {
			if(trgs[i].rdr == nmsk) {
				trgs[i].msk = true;

/*
			} else if(trgs[i].rdr == rdr) {
				trgs[i].cnd = cnt;
				trgs[i].thrsh = 0;
*/
			} else if(trgs[i].rdr == sy) {
				tpt = " ";
				lt_tpt = true;
			};
		};
		lt_tpt = lt_tpt || trgs[i].msk;
		n_msk = n_msk && (trgs[i].rdr_mssg != tpt);
	};
	lt_tpt = lt_tpt || n_msk;
	if(lt_tpt) {
		return tpt;
	} else if(tpt == "") {
		return stndrd_rspns;
	} else return nt_yt;
};
int NPC::gtTrggrCnt() {
	int tpt = 0;
	for(int i(0); i < trgs.size(); i++) {
		tpt += trgs[i].msk;
	};
	return tpt;
};
int NPC::gtMxmmTrggrCnt() { /*
	int tpt = 0;
	for(int i(0); i < trgs.size(); i++) {
		if(trgs[i].mx == dd) tpt += stoi( trgs[i].rdr_mssg );
	};
	return tpt;
	*/
	return mxmm_trggrs;
};
vector<string> NPC::ldTrggr(vector<string> chts, int i) {
	if(chts[i].size() > 8) {
		chts[i].erase(0, 8);
		string cndtn;
		trgs.push_back( {0, "", hv, false, "", nmsk} );
		/*
		struct trggrs {
			//Conditions
			int thrsh 		//threshhold
			string cnd_mssg	//have item
			int cnd			//condition
			
			//Orders
			bool msk 		//mask
			string rdr_mssg	//order message
			int rdr			//order
		};
		*/
		//Split Trigger load order and save condition
		if( chts[i].find("unmask") != string::npos ) {
			int rdr_txt_plc = chts[i].find("unmask");
			
			trgs.back().rdr_mssg = chts[i+2];
			trgs.back().rdr = nmsk;
			
			cndtn = chts[i].substr(0, rdr_txt_plc - 1 );
			chts.erase( chts.begin() + i );
		
		} else if( chts[i].find("drop") != string::npos ) {
			int rdr_txt_plc = chts[i].find("drop");
			
			trgs.back().rdr_mssg = chts[i].substr( rdr_txt_plc );
			trgs.back().rdr = rdr;
			
			cndtn = chts[i].substr(0, rdr_txt_plc - 1 );
			chts.erase( chts.begin() + i );
		
		} else if( chts[i].find("say") != string::npos ) {
			int rdr_txt_plc = chts[i].find("say");
			
			trgs.back().rdr_mssg = chts[i+1];
			trgs.back().rdr = sy;

			cndtn = chts[i].substr(0, rdr_txt_plc - 1 );
			chts.erase( chts.begin() + i );
			chts.erase( chts.begin() + i );
		
		} else if( chts[i].find("add") != string::npos ) {
			int rdr_txt_plc = chts[i].find("add");
			
			trgs.back().rdr_mssg = chts[i].substr(rdr_txt_plc + 4);
			trgs.back().rdr = dd;

			cndtn = chts[i].substr(0, rdr_txt_plc - 1);
			chts.erase( chts.begin() + i );
		
		} else if( chts[i].find("maximum") != string::npos ) {
			int rdr_txt_plc = chts[i].find("maximum");

			trgs.back().rdr_mssg = chts[i].substr(rdr_txt_plc + 8);
			mxmm_trggrs += stoi(trgs.back().rdr_mssg);
			
			trgs.pop_back();
			chts.erase( chts.begin() + i );
		
		} else if( chts[i].find("lose") != string::npos ) {
			int rdr_txt_plc = chts[i].find("lose");
			
			trgs.back().rdr = ls;

			cndtn = chts[i].substr(0, rdr_txt_plc - 1);
			chts.erase( chts.begin() + i );
		};
		ldCndtn(cndtn);
	};
	return chts;
};
void NPC::ldCndtn(string cndtn) {
	if( cndtn.substr(0, 4) == "have" ) {
		trgs.back().cnd_mssg = cndtn.substr(5);
		trgs.back().cnd = hv;

	} else if( cndtn.substr(0, 7) == "counter" ) {
		trgs.back().cnd = cnt;
		trgs.back().thrsh = stoi( cndtn.substr(8) );

	} else if( cndtn.substr(0, 4) == "hear" ) {
		trgs.back().cnd_mssg = cndtn.substr(5);
		trgs.back().cnd = hr;

	} else if( cndtn.substr(0, 4) == "said" ) {
		trgs.back().cnd_mssg = cndtn.substr(5);
		trgs.back().cnd = sd;
	};
};
vector<trggr> NPC::chckCndtn(string npt, string tpt_s, int cntr) {
	vector<trggr> tpt;
	for(int i(0); i < trgs.size(); i++) {

		//Is this a "have an item" condition?
		if(trgs[i].cnd == hv) {
			for(int j(0); j < tms.size(); j++) {
				if(tms[j]->gtNm() == trgs[i].cnd_mssg) {
					tpt.push_back(trgs[i]);
					trgs.erase(trgs.begin() + i);
					i--;
					break;
				};
			};
		//Is this a "threshhold" condition?
		} else if(trgs[i].cnd == cnt) {
			if(trgs[i].thrsh <= cntr) {
				tpt.push_back(trgs[i]);
				trgs.erase(trgs.begin() + i);
				i--;
			};

		//Is this a "hear a phrase" condition?
		} else if(trgs[i].cnd == hr) {
			if(npt == trgs[i].cnd_mssg) {
				tpt.push_back(trgs[i]);
				trgs.erase(trgs.begin() + i);
				i--;
			};
		
		//Npc said a phrase
		} else if(trgs[i].cnd == sd) {
			if(tpt_s == trgs[i].cnd_mssg) {
				tpt.push_back(trgs[i]);
				trgs.erase(trgs.begin() + i);
				i--;
			};
		};
	};
	return tpt;
};
void NPC::stTrgrMsk(trggr trg) {
	for(int i(0); i < trgs.size(); i++) {
		if(	trgs[i].thrsh == trg.thrsh &&
			trgs[i].cnd_mssg == trg.cnd_mssg &&
			trgs[i].cnd == trg.cnd &&
			trgs[i].rdr_mssg == trg.rdr_mssg &&
			trgs[i].rdr == trg.rdr) {
				trgs[i] = trg;
			};
	};
};
/*
struct trggrs {
	//Conditions
	int thrsh 		//threshhold
	string cnd_mssg	//have item
	int cnd			//condition
	
	//Orders
	bool msk 		//mask
	string rdr_mssg	//order message
	int rdr			//order
};
*/
#endif
