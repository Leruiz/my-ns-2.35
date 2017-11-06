#include "Replica.h"
#include <algorithm>
#include "random.h"
#include<string.h>
#include <iostream>
using namespace std;

/*
static class ReplicaClass : public TclClass {
public:
	ReplicaClass() : TclClass("Replica") {}
	TclObject* create(int, const char*const*) {
		return (new Replica());
	}
} sat_class_Replica;
*/

//quick sort
int part(int a[], int p, int r){
	int key = a[r];
	int i = p-1, j = p;
		while(j <= r){
			if(a[j] <= key){
		    	i ++;
		        if(i < j){
		        	a[i] = a[i] + a[j];
		        	a[j] = a[i] - a[j];
		        	a[i] = a[i] - a[j];
		        }
			}
		    j ++;
		}
	return i;
}

void quick_sort(int a[], int p, int r){
	if(p >= r) return;
	int idx = part(a, p, r);
	quick_sort(a, p, idx - 1);
	quick_sort(a, idx + 1, r);
}

void qsort(int* a, int n){
	quick_sort(a, 0, n-1);
}

//******************************************************
Replica::~Replica() {
	printf("into ~Replica\n");

	delete []rep;
	printf("out ~Replica\n");
}


Replica::Replica(int dc): dcsize(dc), re_dcsize(dc){
	cout<<"into Replica constuctor!"<<endl;
	cout<<"now = "<<NOW<<endl;

	rep = new int [dc] ();
	for(int i=0; i<=dc-1; i++)
			rep[i] = i;
	cout<<"out Replica constructor!"<<endl;
}

Replica* Replica::ptrcopy(){
	Replica* p = new Replica();
	p -> dcsize = dcsize;
	p -> re_dcsize = re_dcsize;

	p -> rep = new int [dcsize];
	memcpy(p->rep, rep, sizeof(p->rep));

//	p -> rep = rep;
	return p;
}

static int soliton_val[10]  = { 1000, 6000, 7667, 8500, 9000, 9333, 9571, 9750 , 9889, 10000};

void Replica::SolitonRep(){
	int num = (int)(Random::uniform() * 10000);
	int i = 0;
	while(i <= 9){
		if(num <= soliton_val[i])
			break;
		else i ++;
	}
	if(i < 10) i ++;
	re_dcsize = (dcsize / 10) * i;
	if(re_dcsize == 0) re_dcsize = 1;
	if(re_dcsize == dcsize) re_dcsize --;

	for(int i= 0;i <= dcsize-1; i++)
		rep[i] = i;

	for(int k = re_dcsize+1; k <= dcsize; k ++){
		int tem = (int)(Random::uniform()*k) + 1;
		if(tem <= re_dcsize)
		{
			rep[tem-1] = k - 1;
			rep[k-1] = tem -1;
		}
	}
	qsort(rep, re_dcsize);
}

void Replica:: HalfRep(int re_dc){
	cout<<"into Replica::HalfRep!\n"<<"now = "<<NOW<<endl;
	re_dcsize = re_dc;

	for(int i=0; i<=dcsize-1; i++)
		rep[i] = i;
	cout<<"out Replica::HalfRep!\n";
}

void Replica::FullRep(){
	cout<<"into Replica::FullRep!\n"<<"now = "<<NOW<<endl;

	re_dcsize = dcsize;

	for(int i=0; i<=dcsize-1; i++)
		rep[i] = i;
	cout<<"out Replica::FullRep!\n";
}

