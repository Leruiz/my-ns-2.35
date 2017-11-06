#include "CRDSA.h"
#include "random.h"
#include <iostream>

using  namespace std;
static class CRDSAClass : public TclClass {
public:
	CRDSAClass() : TclClass("Mac/Sat/CRDSA") {}
	TclObject* create(int, const char*const*) {
		return (new CRDSA());
	}
} sat_class_CRDSA;


void SICTimer::expire(Event* e){

	t_->SIC();
}

/*==========================================================================*/
/*
 * _CRDSA
 */




CRDSA::CRDSA() : SatMac(),sic_(this),timeron(false)
{
	bind("frame_length_",&frame_length_);
	bind_time("slot_duration_",&slot_duration_);

}


void CRDSA::sendUp(Packet* p)
{
	hdr_mac* mh = HDR_MAC(p);
	int dst;

	int stamp1=p->getStamp1();
	int stamp2=p->getStamp2();
//	vector<int> loca_stamp_=p->getLocationStamp();

	buffer_.insert(pair<int,Packet*>(stamp1,p));
	buffer_.insert(pair<int,Packet*>(stamp2,p));

	if(!timeron){
	sic_.sched(0.001+slot_duration_*frame_length_);
	timeron=true;
	}

}

void CRDSA::sendDown(Packet* p)
{
	if(NOW>slot_duration_*frame_length_){
		drop(p);
		return;
	}
	double txt;


	int num1=Random::integer(frame_length_);
	p->setStamp1(num1);


	int num2=Random::integer(frame_length_);
	if(num2==num1){
		assert("the same stamp!");
	}else{
		p->setStamp2(num2);
	}

	// compute transmission delay:
	int packetsize_ = HDR_CMN(p)->size() + LINK_HDRSIZE;
	assert (bandwidth_ != 0);
	txt = txtime(packetsize_);
    HDR_MAC(p)->txtime() = txt;

	// Send the packet down
	downtarget_->recv(p, this);

	// Set a timer-- if we do not hear our own transmission within this
	// interval (and cancel the timer), the send_timer will expire and
	// we will backoff and retransmit.
	//send_timer_.resched(send_timeout_ + txt);

}

void CRDSA::SIC(){
	vector<Packet*> rslv_pkt_;

	bool stop=false;
	while(!stop && !buffer_.empty()){

		bool exist_deg_1_=false;
		for(int i=0;i<=frame_length_;i++){

			if(buffer_.count(i)==1){
				exist_deg_1_=true;
				Packet* p=(*((buffer_.equal_range(i)).first)).second;
				rslv_pkt_.push_back(p);
				int stamp[2];
				stamp[0]=p->getStamp1();
				stamp[1]=p->getStamp2();
				for(int ii=0;ii<=1;ii++){
					int j=stamp[ii];
					multimap<int, Packet*>::iterator bit1=buffer_.equal_range(j).first;
					multimap<int, Packet*>::iterator bit2=buffer_.equal_range(j).second;
					for(multimap<int, Packet*>::iterator biter=bit1;biter!=bit2;biter++){
						if((*biter).second==p){
							buffer_.erase(biter);
							break;
						}
					}
				}
			}
		}
		if(!exist_deg_1_) stop=true;

	}

	//send up these packets

	if(!rslv_pkt_.empty()){

	for(vector<Packet*>::iterator iter=rslv_pkt_.begin();iter!=rslv_pkt_.end();iter++){

		Packet* p=*iter;
		if (!p) return; // No packet to free or send up.

		hdr_mac* mh = HDR_MAC(p);
		int dst = this->hdr_dst((char*)mh); // mac destination address
		int src = this->hdr_src((char*)mh); // mac source address


			// wait for processing delay (delay_) to send packet upwards
			Scheduler::instance().schedule(uptarget_, p, delay_);

	}
	}
}

// Called when contention period ends

