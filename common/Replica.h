#ifndef COMMON_REPLICA_H_
#define COMMON_REPLICA_H_
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include "scheduler.h"
#include "object.h"
#include "lib/bsd-list.h"
#include "packet-stamp.h"
#include "ns-process.h"
#include "stdlib.h"
#include "unistd.h"
using namespace std;

class Replica:public Event{
private:
	int dcsize;
	int re_dcsize;
	int* rep;
public:
	//Replica() : dcsize (0), re_dcsize (0), rep (vector<int> ()) {}
	Replica() : dcsize (0), re_dcsize (0), rep (0) {}
	Replica(int dc);
	~Replica();

	void SolitonRep();
	void HalfRep(int re_dc);
	void FullRep();

	Replica* ptrcopy();

	int getDcsize(){ return dcsize;}
	int getReDcsize(){ return re_dcsize;}
	//vector < int > getRep() {return rep;}
	int* getRep() { return rep;}

};
#endif /* COMMON_REPLICA_H_ */
