#include "ZDPA.h"
#include "sattrace.h"
#include "satposition.h"
#include "satgeometry.h"
#include "satnode.h"
#include "satroute.h"
#include "errmodel.h"
#include "sat-hdlc.h"
#include  "stdlib.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include "SolitonAveLen.h"

using namespace std;

static class ZDPAClass : public TclClass {
public:
	ZDPAClass() : TclClass("Mac/Sat/ZDPA") {}
	TclObject* create(int, const char*const*) {
		return (new ZDPA());
	}
} sat_class_ZDPA;

vector<Packet*> ZDecode::zdecode(unsigned int start_bit_, unsigned int end_bit_){

	//compute the number of bits in class one
	int num_class_one = floor(168 * t_->par_ratio);
	if(num_class_one == 168)
		num_class_one --;
	else if(num_class_one == 0)
		num_class_one = 1;

//		cout<<"into ZDecode::zdecode"<<endl;
	//	cout<<"Now = "<<NOW<<endl;
		multimap<unsigned int, PacketBit*> rcv_bit_;
		map<unsigned int,PacketNode* > temp_pktlist_;
		for(multimap<unsigned int, PacketBit*>::iterator iter=t_->rcv_bit_buffer_.begin();iter!=t_->rcv_bit_buffer_.end();iter++){
			if((*iter).first>=start_bit_ && (*iter).first <= end_bit_){
				if(! (*iter).second->pn){
					cout<<"zdecode:: pn is null"<<endl;
					abort();
				}
				rcv_bit_.insert(*iter);
			}
		}

		//ZigZag Decoding
		int decode_iter_num=0;
		
		int arr[end_bit_ - start_bit_ +1];
		for(int i=0;i<=end_bit_-start_bit_+1-1;i++){
			arr[i] = 1;	
		}		

		while(decode_iter_num<=t_->decode_iter_max){
			bool flag_=false;//Is there any bit_slot of which degree equals 1?
			for(unsigned int i=start_bit_;i<=end_bit_;i++){
				//i--current bit_slot
				if(rcv_bit_.count(i)==0) arr[i-start_bit_] = 0;
				if(rcv_bit_.count(i)==1&& arr[i-start_bit_] == 1 ){//decode from here

					flag_=true;// be able to decode

					bool state_flag_ = false;

					pair<unsigned int, PacketBit*> p=*(rcv_bit_.equal_range(i).first);
					PacketBit* pb_=p.second;//resolved pkt-bit

					//take down the Resolved bit
					 if(temp_pktlist_.find(pb_->pn->uid_) == temp_pktlist_.end()){
						 temp_pktlist_.insert(pair<unsigned int, PacketNode*>(pb_->pn->uid_,pb_->pn));
                     }
//************************************************************************
					 PacketNode* ttpn = temp_pktlist_[pb_->pn->uid_];
					 if(t_->construction == 0){
						 //ttpn->Noded_bit_counter ++;
						 if(!ttpn->reveal_bit[pb_->bit_id])
						 {
							 ttpn->Noded_bit_counter ++;
							 ttpn->reveal_bit[pb_->bit_id] = true;
							 SBNumClassOne::sb_decoded ++;
						 }

						 if(pb_->bit_id < num_class_one){
							 SBNumClassOne::sb_of_class_one ++;
							 ttpn->node_count_one ++;
						 }else{
							 SBNumClassTwo::sb_of_class_two ++;
							 ttpn->node_count_two ++;
						 }
					 }else if(t_->construction == 1){
						 if(!ttpn->reveal_bit[pb_->bit_id]){
							 	 ttpn->Noded_bit_counter ++;
						 		ttpn->reveal_bit[pb_->bit_id] = true;
						 		SBNumClassOne::sb_decoded ++;
						 }
					 }

				 		/*if(SBNumClassOne::decosb_.find(ttpn->p->own_uid_) == SBNumClassOne::decosb_.end()){
				 			vector<bool> tv (ttpn->p->getDcsize(), false);
				 			tv[pb_->bit_id] = true;
				 			SBNumClassOne::decosb_.insert(make_pair(ttpn->p->own_uid_, tv));
				 			SBNumClassOne::sb_decoded ++;
				 		}else{
				 			vector<bool> tv = SBNumClassOne::decosb_[ttpn->p->own_uid_];
				 			if(!tv[pb_->bit_id]){
				 				SBNumClassOne::sb_decoded ++;
				 				SBNumClassOne::decosb_.find(ttpn->p->own_uid_)->second[pb_->bit_id] = true;
				 			}
				 		}*/
//***********************************************************************
					// delete the bit in rcv_bit_
					PacketNode* tpn=pb_->pn;

					vector<unsigned int> location_vec;
//					cout<<tpn->uid_<<endl;

//					cout<< (tpn->location_pool).size()<<endl;
					for(vector<unsigned int>::iterator iter=tpn->location_pool.begin();iter!=tpn->location_pool.end();iter++){
						unsigned int temp=*iter;
						location_vec.push_back(temp);
					}

					for(vector<unsigned int>::iterator it=location_vec.begin();it!=location_vec.end();it++){

						//*it--corresponding bit_slot
						unsigned int location_=pb_->bit_id+(*it);

						if(location_ >end_bit_) state_flag_ = true;

						if(location_!= i){
	                        pair<multimap<unsigned int, PacketBit*>::iterator,multimap<unsigned int, PacketBit*>::iterator> pitt
	                        	=rcv_bit_.equal_range(location_);
	                        multimap<unsigned int, PacketBit*>::iterator itt=pitt.first;

						while(itt!=pitt.second){
	                        PacketBit* tpb=(*itt).second;

							if(tpb->pn->uid_ == pb_->pn->uid_ && tpb->bit_id == pb_->bit_id ){
//								delete (*itt).second;
								(*itt).second=NULL;
								rcv_bit_.erase(itt);
								break;
							}
							itt++;
						}
						}
					}

						(*(rcv_bit_.equal_range(i).first)).second = NULL;
						rcv_bit_.erase(i);
						arr[i-start_bit_] = 0;
				}
			}

			if(!flag_ ){//rcv_bit_ is not empty and no bit_slot's degree=1 => decoding cannot be executed again
				//decoding stops
				break;
			}
			decode_iter_num++;
			//if(NOW >= 2.01953)
			//cout<<" decode_iter_num = "<< decode_iter_num<<endl;
		}
		// check which packets that have been Noded
		vector<Packet*> ans;
		for(map<unsigned int, PacketNode*>::iterator iter=temp_pktlist_.begin();iter!=temp_pktlist_.end();iter++)
		{
			PacketNode* tpn = (*iter).second;
			if(t_->construction == 0){
				if(tpn->node_count_one == num_class_one )
					SBNumClassOne::reveal_pkt_one.push_back(tpn->uid_);
				if(tpn->node_count_two == tpn->dcsize - num_class_one)
					SBNumClassTwo::reveal_pkt_two.push_back(tpn->uid_);
			}

			if((*iter).second->Noded_bit_counter==(*iter).second->dcsize){

				// successfully decoded
				Packet* p=(*iter).second->p;
				hdr_cmn* pch=hdr_cmn::access(p);
				if(!pch) cout<<"pch is null"<<endl;
				ans.push_back(p);
			}

		}

		return ans;
		}

int optimal_num = 0;
unsigned optimal_d = 0;

void ZDecodeTimer::expire(Event* e){
//	cout<<"into ZDecodeTimer::expire"<<endl;
//	cout<<"Now = "<<NOW<<endl;
//	cout<<"Event uid = "<<e->uid_<<endl;
	///zigzag decoding process;

	unsigned int end_bit_=floor(NOW/(t_->bit_duration));
	unsigned int start_bit_=end_bit_-t_->windowt+1;

	vector<Packet*> ans=t_->zdecoder_.zdecode(start_bit_,end_bit_);
	if(!ans.empty()) t_->sendupZDPackets(ans);
	t_->decode_timer_.resched(t_->deltat*t_->bit_duration);
}

void ZLwrTimer::expire(Event* e)
{
		if(Random::uniform()<(1-t_->prob)){//
			t_->snd_pkt_->halfRetrans(t_->par_ratio);
		}else{
			t_->snd_pkt_->fullRetrans();
		}
		Packet* p =t_->snd_pkt_;
		t_->downtarget_->recv(p, t_);

}

/*==========================================================================*/
/*
 * _ZDPA
 */

int num_rcvd_bits_ = 0; 
set<int> ZDPA::succ_pkt_set_;
ZDPA::ZDPA() : SatMac(), tx_state_(MAC_IDLE),
    rx_state_(MAC_IDLE), rtx_(0), end_of_contention_(0), buffered_bit_num_(0),
    decode_timer_(this),zdecoder_(this),timer_on_(false) ,lwr_timer_(this),buffer_size_(0),optimal_deltat(0)
{
	bind_time("mean_backoff_", &mean_backoff_);
	bind("rtx_limit_", &rtx_limit_);
	bind_time("send_timeout_", &send_timeout_);

        // new configuration
	bind("prob",&prob);
	bind("k",&k);
        //bind("half_retrans_max", &half_retrans_max);
        bind("par_ratio",&par_ratio);
        bind_time("bit_duration",&bit_duration);
        bind("windowt",&windowt);
        bind("deltat",&deltat);
        bind("decode_iter_max",&decode_iter_max);
        bind("buffer_max",&buffer_max);
        bind("construction", &construction); // construction = 0: turncated; =1: random
        //decode_timer_.sched(bit_duration*windowt);

 }


ZDPA::~ZDPA(){
	//release rcv_bit_buffer_
	for(multimap<unsigned int, PacketBit* >::iterator iter=rcv_bit_buffer_.begin();iter!=rcv_bit_buffer_.end();iter++){
		delete (*iter).second;
		(*iter).second = NULL;
	}

	//RELEASE  pktlist_buffer_
	for(map<unsigned int , PacketNode*>::iterator iter=pktlist_buffer_.begin();iter!=pktlist_buffer_.end();iter++){
		delete (*iter).second;
		(*iter).second = NULL;
	}
}

void ZDPA::insertPacket(Packet* p,unsigned int bit_slot)
{
	//int* tmp = p -> getRep();
	int re_dcsize = p ->getReDcsize();
	vector<int> tmp (re_dcsize, 0);
	for(int i=0; i<=re_dcsize-1; i++)
		tmp[i] = i;
	if(construction == 1 && re_dcsize != p->getDcsize()){
		//solinton and partial

		SolitonAveLen::rep_num_ ++;
		SolitonAveLen::sum_len_ += re_dcsize;

		for(int k = re_dcsize+1; k <= p->getDcsize(); k ++){
			int tem = (int)(Random::uniform()*k) + 1;
			if(tem <= re_dcsize)
				tmp[tem-1] = k - 1;
		}
		sort(tmp.begin(), tmp.end());
	}

//cout<<"into insertPacket"<<endl;
//inserted into buffer_

	buffer_size_++;

	hdr_cmn* ch=HDR_CMN(p);
	int uid_=ch->uid_;

//inserted into pktlist
	if(pktlist_buffer_.find(uid_)==pktlist_buffer_.end() ){

		PacketNode* pn=new PacketNode;
		pn->p=p->copy();
		pn->uid_=uid_;
		pn->dcsize=p->getDcsize();
		pn->Noded_bit_counter=0;
		pn->location_pool.push_back(bit_slot);
		pktlist_buffer_.insert(pair<int, PacketNode*>(uid_,pn));

		if(pn->location_pool.empty()){
					cout<<"insertPacketA::pn->location_pool is null"<<endl;
					abort();
				}

		for(int j=0;j<=p->re_dcsize-1;j++){
						PacketBit* pb = new PacketBit(pn);
						pb->bit_id= tmp[j];
						//pb->bit_id= j;
						rcv_bit_buffer_.insert(make_pair(bit_slot+pb->bit_id,pb));
						if(! pb->pn){
							cout<<"ZDPA.cc:245 pb->pn_ is null"<<endl;
												abort();
						}
				}

	}else{
		(*pktlist_buffer_.find(uid_)).second->location_pool.push_back(bit_slot);
		if( (*pktlist_buffer_.find(uid_)).second->location_pool.empty()){
					cout<<"insertPacketB::pn->location_pool is null"<<endl;
					abort();
				}

		for(int j=0;j<=p->re_dcsize-1;j++){
						PacketBit* pb = new PacketBit(pktlist_buffer_[uid_]);

						pb->bit_id = tmp[j];
						//pb->bit_id = j;
						rcv_bit_buffer_.insert(make_pair (bit_slot+pb->bit_id,pb));

						if(! pb->pn) {
							cout<<"insert :: pb->pn is null"<<endl;
							abort();
						}
				}
	}
	//inserted into rcv_bits
	//cout<<"out insertPacket"<<endl<<endl;
	return;
}



void ZDPA::sendupZDPackets(vector<Packet*> &vec){

//	cout<<"into sendupZDPackets"<<endl;

	if(vec.empty()) return;

//	rx_state_ = MAC_IDLE;
	for(vector<Packet*>::iterator iter=vec.begin();iter!=vec.end();iter++){
		Packet* p=*iter;
		if (!p) {
			cout<<"sendupZDPackets: the packet is null!"<<endl;
			break;
		}
		hdr_cmn* ch=hdr_cmn::access(p);
		if(!ch) {
			cout<<"ch is null!"<<endl;
		}
		int uid_=ch->uid_;
		map<unsigned int, PacketNode*>::iterator pn_it=pktlist_buffer_.equal_range(uid_).first;
		if(pn_it == pktlist_buffer_.end()) continue;
		PacketNode* pn=(*pn_it).second;
		vector<unsigned int > location_pool_=pn->location_pool;

		//remove from rcv_bit_buffer
		multimap<unsigned int, PacketBit*>::iterator curr_it=rcv_bit_buffer_.begin();
		multimap<unsigned int, PacketBit*>::iterator next_it=curr_it;

		next_it++;
		int rbf=rcv_bit_buffer_.size();
		int i=0;
		while(i<=rbf-1){
			if((*curr_it).second->pn->uid_ ==uid_){
				delete (*curr_it).second;
				(*curr_it).second = NULL;
				rcv_bit_buffer_.erase(curr_it);
				curr_it=next_it;
				next_it++;

			}else{
				curr_it++;
				next_it++;
			}
			i++;
		}

		//remove from pktlist_buffer_
		map<unsigned int, PacketNode*>::iterator pb_it=pktlist_buffer_.equal_range(uid_).first;
		if((*pb_it).second->refnum == 0){
			delete (*pb_it).second;
			(*pb_it).second = NULL;
			pktlist_buffer_.erase(uid_);
		}

			if (!p)
				return; // No packet to free or send up.

			hdr_mac* mh = HDR_MAC(p);
			int dst = this->hdr_dst((char*)mh); // mac destination address
			int src = this->hdr_src((char*)mh); // mac source address

			if (((u_int32_t)dst != MAC_BROADCAST) && (dst != index_) &&
		    	    (src != index_)) {
				drop(p); // Packet not intended for our station
				return;
			}
			if (src == index_) {
				// received our own packet: free up transmit side, drop this
				// packet, and perform callback to queue which is blocked
				if (!callback_) {
					printf("Error, queue callback_ is not valid\n");
					exit(1);
				}
				hdr_cmn * ch  = hdr_cmn::access(p);
				hdr_cmn * chs = hdr_cmn::access(snd_pkt_);
				if(ch->uid_ == chs->uid_ ){
					send_timer_.force_cancel();
					tx_state_ = MAC_IDLE;
					rtx_ = 0;
					drop(snd_pkt_); // Free the packet cached for retransmission
				}	
				resume(p);
				
			} else {

				if(p->re_dcsize == p->dcsize && succ_pkt_set_.find(uid_)==succ_pkt_set_.end()){//the pkt has not been received
					num_rcvd_bits_ += p->dcsize;
					//cout<<"num_rcvd_bits_ = "<<num_rcvd_bits_<<endl;

					RetransTimes::success_retrans_times_sum_ += p->retrans_;

					Scheduler::instance().schedule(uptarget_, p, delay_);
					succ_pkt_set_.insert(uid_);
				}else{
					drop(p);
				}
				// wait for processing delay (delay_) to send packet upwards
//				Scheduler::instance().schedule(uptarget_, p, delay_);
			}
		}
//cout<<"out sendupZDPackets"<<endl<<endl;
	}

void ZDPA::releaseBuffer()
{
}
///************************************************************************************************

void ZDPA::send_timer()
{//cout<<"into send timer"<<endl;
	switch (tx_state_) {

	case MAC_SEND:
		// We've timed out on send-- back off
		// means that last transmission failed, so having to backoff and retransmitting
		backoff();
		break;
	case MAC_COLL:
		// Our backoff timer has expired-- begin resending
		if(snd_pkt_->getRetansCounter() <= k){
			if(Random::uniform()<1-prob){//
				if(construction == 0) // truncated
						snd_pkt_->halfRetrans(par_ratio);
				else if(construction == 1) //soliton
						snd_pkt_ -> ParSoliton();
				else if(construction == 2) //DRA
					snd_pkt_->fullRetrans();
			}
		}else{
			snd_pkt_->originRetrans();
		}
		sendDown(snd_pkt_);
		break;
	default:
		printf("Error: wrong tx_state in unslotted aloha: %d\n",
		    tx_state_);
		break;

	}
//cout<<"out send timer"<<endl;
}

void ZDPA::recv_timer()
{//cout<<"into recv timer"<<endl;

if(! rcv_pkt_) {
	cout<<"rcv_pkt_ is null!"<<endl;
	return;
}
	switch (rx_state_) {

	case MAC_RECV:
		// We've successfully waited out the reception
		end_of_contention(rcv_pkt_);
		break;
	default:
		printf("Error: wrong rx_state in unslotted aloha: %d\n",
		    rx_state_);
		break;
	}
//cout<<"out recv timer"<<endl<<endl;
}

void ZDPA::sendUp(Packet* p)
{
	//cout<<"into ZDPA::sendup"<<endl;
	//cout<<"now= "<<NOW<<endl;
	unsigned int key_ = floor(NOW/bit_duration);

	if(p->dcsize == 0 || p->re_dcsize == 0) {
		hdr_cmn* ch = hdr_cmn::access(p);
		abort();
	}

	this->insertPacket(p,key_);

	hdr_cmn* ch=hdr_cmn::access(p);
	hdr_mac* mh = HDR_MAC(p);

	int dst;

	if (p->re_dcsize == p->dcsize && rx_state_ == MAC_IDLE) {
		// First bit of packet has arrived-- wait for
		// txtime to make sure no collisions occur
		//the packet can be received successfully
		rcv_pkt_ = p;
		end_of_contention_ = NOW + mh->txtime();//the time of current packet finishing transmission
		rx_state_ = MAC_RECV;
		recv_timer_.resched(mh->txtime());

	} else {
		// Collision: figure out if contention phase must be lengthened
		if ( (NOW + mh->txtime()) > end_of_contention_ ) {
			recv_timer_.resched(mh->txtime());//if this recv_timer_ is set, last rcv_timer_ will be cancelled
											// so the contention phase is lengthened
		}
		//*************************************************************************************
			if(!timer_on_){
			 decode_timer_.sched(0.0009);
			 timer_on_=true;
			}
	}
	//cout<<"out ZDPA::send up"<<endl<<endl;
}

void ZDPA::sendDown(Packet* p)
{
	p->retrans_ ++;
	RetransTimes::prac_sent_pkt_ ++;
	//cout<<"into ZDPA::send down"<<endl;
	// cout<<"now= "<<NOW<<endl<<endl<<endl;
	double txt;
	double retrans_interval;
	if(p-> getRetansCounter() > k)
	{
		// exceed the maximal retransmission times
		retrans_interval = send_timeout_;
	}else
	{
		if(construction == 2)
		{
			//DRA protocol
			int interval = windowt / (k - 1);
			int random_interval = ((int)(Random::uniform() * interval)) % interval;
			retrans_interval = (interval + random_interval) * bit_duration;
		}else
			retrans_interval = (windowt / (k - 1)) * bit_duration;
	}

	// compute transmission delay:
	int packetsize_ = HDR_CMN(p)->size() + LINK_HDRSIZE;
	assert (bandwidth_ != 0);
	//txt = txtime(packetsize_);
	txt = p->re_dcsize * bit_duration;
	HDR_MAC(p)->txtime() = txt;
        hdr_cmn* ch=hdr_cmn::access(p);
 	//cout<<"pkt uid_ = "<<ch->uid_<<endl;
	
	// Send the packet down
	tx_state_ = MAC_SEND;
	snd_pkt_ = p->copy();  // save a copy in case it gets retransmitted

	PracOfferedTrafficLoad::num_sent_bits_ += p->re_dcsize;
	downtarget_->recv(p, this);
	// Set a timer-- if we do not hear our own transmission within this
	// interval (and cancel the timer), the send_timer will expire and
	// we will backoff and retransmit.

	send_timer_.resched(retrans_interval + txt);
	//cout<<"out send down"<<endl<<endl;
}

// Called when contention period ends
void ZDPA::end_of_contention(Packet* p)
{//cout<<"into end of contention"<<endl;

	rx_state_ = MAC_IDLE;
	if (!p)
		return; // No packet to free or send up.

	hdr_mac* mh = HDR_MAC(p);
	int dst = this->hdr_dst((char*)mh); // mac destination address
	int src = this->hdr_src((char*)mh); // mac source address

	if (((u_int32_t)dst != MAC_BROADCAST) && (dst != index_) &&
    	    (src != index_)) {

		drop(p); // Packet not intended for our station
		return;
	}
	if (src == index_) {
		// received our own packet: free up transmit side, drop this
		// packet, and perform callback to queue which is blocked
		if (!callback_) {
			printf("Error, queue callback_ is not valid\n");
			abort();
		}
		hdr_cmn * ch  = hdr_cmn::access(p);
                hdr_cmn * chs = hdr_cmn::access(snd_pkt_);
                if(ch->uid_ == chs->uid_ ){
	                send_timer_.force_cancel();
                        tx_state_ = MAC_IDLE;
                        rtx_ = 0;
                        drop(snd_pkt_); // Free the packet cached for retransmission
                }
                resume(p);

	} else {
		// wait for processing delay (delay_) to send packet upwards
		hdr_cmn* ch=hdr_cmn::access(p);
		int uid_=ch->uid_;
		if(p->re_dcsize == p->dcsize && succ_pkt_set_.find(uid_)==succ_pkt_set_.end()){//the pkt has not been received
			num_rcvd_bits_ += p->dcsize;
		//	cout<<"num_rcvd_bits_ = "<<num_rcvd_bits_<<endl;

			SBNumClassOne::reveal_pkt_one.push_back(p->uid_);
			SBNumClassTwo::reveal_pkt_two.push_back(p->uid_);

			SBNumClassOne::sb_decoded += p->getDcsize();
	 		if(SBNumClassOne::decosb_.find(p->own_uid_) != SBNumClassOne::decosb_.end()){
	 			vector<bool> tv (p->getDcsize(), true);
	 			SBNumClassOne::decosb_[p->own_uid_] = tv;
	 		}


 			RetransTimes::success_retrans_times_sum_ += p->retrans_;

			Scheduler::instance().schedule(uptarget_, p, delay_);
			succ_pkt_set_.insert(uid_);
		}else{
			drop(p);
		}
	}

	//cout<<"out end of contention"<<endl<<endl;
}

void ZDPA::backoff(double delay)
{//cout<<"into backoff"<<endl;
	double backoff_ = Random::exponential(mean_backoff_);

	// if number of retransmissions is within limit, do exponential backoff
	// else drop the packet and resume
	if (++rtx_ <= rtx_limit_) {
		delay_+=backoff_;
		tx_state_ = MAC_COLL;
		send_timer_.resched(delay_);
	} else {
		tx_state_ = MAC_IDLE;
		rtx_ = 0;
		// trace the dropped packet
		if (drop_trace_ && trace_drops_)
			drop_trace_->traceonly(snd_pkt_);
		resume(snd_pkt_);
	}
	//cout<<"out backoff"<<endl<<endl;
}
