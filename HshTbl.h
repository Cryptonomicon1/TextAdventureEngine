#ifndef HSHTBL_H
#define HSHTBL_H

#include<vector>
using std::vector;

#include<string>
using std::string;

#include<memory>
using std::move;
using std::shared_ptr;
using std::make_shared;

//#include<iostream>
//using std::cout;

#include"Mrmr3.h"

template <class tp>
class HshTbl : private Mrmr3 {
public:
	HshTbl();
	~HshTbl();
	void Gv(string, tp);	//Give
	tp Tk(string);			//Take
	void Rs(string);		//Erase
private:
	//Variables
	struct cll {						//Cell
		shared_ptr<cll> nxt = nullptr;	//Next Cell
		shared_ptr<cll> bfr = nullptr;	//Before Cell
		tp vl;							//Value
		string nm;						//Name
	};
	vector<shared_ptr<cll>> clls;
	vector<shared_ptr<cll>> cll_stck;
	int sz_ndx;							//Size Index
	int ntr_cnt;						//Entry Count
	const static int sd = 7;			//Seed
	long int szs[19];

	//Functions
	shared_ptr<cll> Fnd(string);			//Find
	void Rsz();								//Resize
	vector<shared_ptr<cll>>
	MptCllPntrs(vector<shared_ptr<cll>>);	//Empty Cell Pointers
	void Ntr(shared_ptr<cll>);				//Enter

};
template <class tp>
HshTbl<tp>::HshTbl() {
	szs[0]=11; szs[1]=31; szs[2]=101; szs[3]=307; szs[4]=911;
	szs[5]=2729; szs[6]=8191; szs[7]=24547; szs[8]=73937;
	szs[9]=220889; szs[10]=662671; szs[11]=1987987;
	szs[12]=5963981; szs[13]=17891857; szs[14]=53675543;
	szs[15]=161026637; szs[16]=483079873; szs[17]=1449239641;
	szs[18]=4294967296;

	sz_ndx = 3;
	clls.resize(szs[sz_ndx]);
};
template <class tp>
HshTbl<tp>::~HshTbl() {
};
template <class tp>
void HshTbl<tp>::Gv(string ttl, tp nmbr) {
	shared_ptr<cll> whr = Fnd(ttl);			//Where
	//cout << ttl << " " << Hsh(ttl, sd) % szs[sz_ndx] << "\n";
	if(whr != nullptr) {
		whr->vl = nmbr;
	} else {
		whr = make_shared<cll> ();
		whr->nm = ttl;
		whr->vl = nmbr;
		Ntr(move(whr));
	};
};
template <class tp>
tp HshTbl<tp>::Tk(string ttl) {
	shared_ptr<cll> whr = Fnd(ttl);			//Where
	if(whr == nullptr) {
		//cout << "Error: Could not Get Value" << endl;
		return "";
	} else {
		return whr->vl;
	};
};
template <class tp>
void HshTbl<tp>::Rs(string ttl) {
	shared_ptr<cll> whr = Fnd(ttl);

	if(whr->bfr != nullptr) {
		whr->nxt->bfr = whr->bfr;
		whr->bfr->nxt = whr->nxt;
		ntr_cnt--;
	} else if(whr->nxt != nullptr) {
		int plc = Hsh(ttl, sd) % szs[sz_ndx];		//Place
		whr->nxt->bfr = nullptr;
		clls[plc] = whr->nxt;
		ntr_cnt--;
	} else {
		int plc = Hsh(ttl, sd) % szs[sz_ndx];		//Place
		clls[plc] = nullptr;
		ntr_cnt--;
	};
};
template <class tp>
shared_ptr<class HshTbl<tp>::cll>  HshTbl<tp>::Fnd(string ttl) {
	int whr = Hsh(ttl, sd) % szs[sz_ndx];
	if(clls[whr] == nullptr) {
		return nullptr;
	} else {
		shared_ptr<cll> lst_cll = clls[whr];	//Last Cell
		while(lst_cll->nm != ttl) {
			if(lst_cll->nxt != nullptr) {
				lst_cll = lst_cll->nxt;
			} else {
				return nullptr;
			};
		};
		return move(lst_cll);
	};
};
template <class tp>
void HshTbl<tp>::Rsz() {
	if(sz_ndx < 18) {
		shared_ptr<cll> tmp;
		for(int i(0); i < szs[sz_ndx]; i++) {
			tmp = clls[i];
			while(tmp != nullptr) {
				cll_stck.push_back(tmp);
				tmp = tmp->nxt;
			};
		};
		sz_ndx++;
		ntr_cnt = 0;
		clls.clear();
		clls.resize(szs[sz_ndx], nullptr);
		cll_stck = MptCllPntrs(cll_stck);
		for(int i(0); i < cll_stck.size(); i++) {
			Ntr(move(cll_stck[i]));
		};
		cll_stck.clear();
	};
};
template <class tp>
vector<shared_ptr<class HshTbl<tp>::cll>>
HshTbl<tp>::MptCllPntrs(vector<shared_ptr<cll>> tmp) {
	for(int i(0); i < tmp.size(); i++) {
		tmp[i]->nxt = nullptr;
		tmp[i]->bfr = nullptr;
	};
	return tmp;
};
template <class tp>
void HshTbl<tp>::Ntr(shared_ptr<cll> whr) {
	ntr_cnt++;
	if(2*ntr_cnt >= szs[sz_ndx]) Rsz();

	int plc = Hsh(whr->nm, sd) % szs[sz_ndx];			//Place
	shared_ptr<cll> tmp = move(clls[plc]);
	clls[plc] = move(whr);
	clls[plc]->nxt = move(tmp);
	if(clls[plc]->nxt != nullptr) {
		clls[plc]->nxt->bfr = clls[plc];
	};
};
#endif
