/* -*-  Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 1999 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the MASH Research
 *      Group at the University of California Berkeley.
 * 4. Neither the name of the University nor of the Research Group may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Contributed by Tom Henderson, UCB Daedalus Research Group, June 1999
 */

#ifndef lint
static const char rcsid[] =
    "@(#) $Header: /cvsroot/nsnam/ns-2/satellite/satlink.cc,v 1.14 2005/09/21 21:45:04 haldar Exp $";
#endif

/*
 * Contains source code for:
 *	SatLinkHead
 *  	SatLL
 * 	SatMac
 * 	SatPhy
 * 	SatChannel
 */

#include "satlink.h"
#include "sattrace.h"
#include "satposition.h"
#include "satgeometry.h"
#include "satnode.h"
#include "satroute.h"
#include "errmodel.h"
#include "sat-hdlc.h"
#include  "stdlib.h"
#include <iostream>

using namespace std;

/*==========================================================================*/
/*
 * _SatLinkHead
 */

static class SatLinkHeadClass : public TclClass {
public:
	SatLinkHeadClass() : TclClass("Connector/LinkHead/Sat") {}
	TclObject* create(int, const char*const*) {
		return (new SatLinkHead);
	}
} class_sat_link_head;

SatLinkHead::SatLinkHead() : linkup_(1), phy_tx_(0), phy_rx_(0), mac_(0), satll_(0), queue_(0), errmodel_(0)
{
}

int SatLinkHead::command(int argc, const char*const* argv)
{
	if (argc == 2) {
	} else if (argc == 3) {
		if (strcmp(argv[1], "set_type") == 0) {
			if (strcmp(argv[2], "geo") == 0) {
				type_ = LINK_GSL_GEO;
				return TCL_OK;
			} else if (strcmp(argv[2], "polar") == 0) {
				type_ = LINK_GSL_POLAR;
				return TCL_OK;
			} else if (strcmp(argv[2], "gsl") == 0) {
				type_ = LINK_GSL;
				return TCL_OK;
			} else if (strcmp(argv[2], "gsl-repeater") == 0) {
				type_ = LINK_GSL_REPEATER;
				return TCL_OK;
			} else if (strcmp(argv[2], "interplane") == 0) {
				type_ = LINK_ISL_INTERPLANE;
				return TCL_OK;
			} else if (strcmp(argv[2], "intraplane") == 0) {
				type_ = LINK_ISL_INTRAPLANE;
				return TCL_OK;
			} else if (strcmp(argv[2], "crossseam") == 0) {
				type_ = LINK_ISL_CROSSSEAM;
				return TCL_OK;
			} else {
				printf("Unknown link type: %s\n", argv[2]);
				exit(1);
			} 
		}
		if (strcmp(argv[1], "setll") == 0) {
			satll_ = (SatLL*) TclObject::lookup(argv[2]);
			if (satll_ == 0)
				return TCL_ERROR;
			return TCL_OK;
		} else if(strcmp(argv[1], "setphytx") == 0) {
			phy_tx_ = (SatPhy*) TclObject::lookup(argv[2]);
			if (phy_tx_ == 0)
				return TCL_ERROR;
			return TCL_OK;
		} else if(strcmp(argv[1], "setphyrx") == 0) {
			phy_rx_ = (SatPhy*) TclObject::lookup(argv[2]);
			if (phy_rx_ == 0)
				return TCL_ERROR;
			return TCL_OK;
		} else if(strcmp(argv[1], "setmac") == 0) {
			mac_ = (SatMac*) TclObject::lookup(argv[2]);
			if (mac_ == 0)
				return TCL_ERROR;
			return TCL_OK;
		} else if(strcmp(argv[1], "setqueue") == 0) {
			queue_ = (Queue*) TclObject::lookup(argv[2]);
			if (queue_ == 0)
				return TCL_ERROR;
			return TCL_OK;
		} else if(strcmp(argv[1], "seterrmodel") == 0) {
			errmodel_ = (ErrorModel*) TclObject::lookup(argv[2]);
			if (errmodel_ == 0)
				return TCL_ERROR;
			return TCL_OK;
		}
	}
	return (LinkHead::command(argc, argv));
}

/*==========================================================================*/
/*
 * _SatLL
 */

static class SatLLClass : public TclClass {
public:
	SatLLClass() : TclClass("LL/Sat") {}
	TclObject* create(int, const char*const*) {
		return (new SatLL);
	}
} sat_class_ll;


void SatLL::recv(Packet* p, Handler* /*h*/)
{
	hdr_cmn *ch = HDR_CMN(p);
	
	/*
	 * Sanity Check
	 */
	assert(initialized());
	
	// If direction = UP, then pass it up the stack
	// Otherwise, set direction to DOWN and pass it down the stack
	if(ch->direction() == hdr_cmn::UP) {
		uptarget_ ? sendUp(p) : drop(p);
		return;
	}

	ch->direction() = hdr_cmn::DOWN;
	sendDown(p);
}
int SatLL::command(int argc, const char*const* argv)
{
	if (argc == 3) {
		if (strcmp(argv[1], "setnode") == 0) {
			satnode_ = (SatNode*) TclObject::lookup(argv[2]);
			return (TCL_OK);
		}
	}
	return LL::command(argc, argv);
}

int SatLL::getRoute(Packet *p)
{
	hdr_cmn *ch = HDR_CMN(p);
        // wired-satellite integration
	if (SatRouteObject::instance().wiredRouting()) {
		
		hdr_ip *ip = HDR_IP(p);
		RouteLogic *routelogic_;
		int next_hopIP = -1; // Initialize in case route not found
		int myaddr_;
		// Wired/satellite integration
		// We need to make sure packet headers are set correctly
		// This code adapted from virtual-classifier.cc
		
		Tcl &tcl = Tcl::instance();
		tcl.evalc("[Simulator instance] get-routelogic");
		routelogic_ = (RouteLogic*) TclObject::lookup(tcl.result());
		char* adst = Address::instance().print_nodeaddr(ip->daddr());
		myaddr_ = satnode()->ragent()->myaddr();
		//char* asrc = Address::instance().print_nodeaddr(h->saddr());
		char* asrc = Address::instance().print_nodeaddr(myaddr_);
		routelogic_->lookup_flat(asrc, adst, next_hopIP);
		delete [] adst;
		delete [] asrc;
		// The following fields are usually set by routeagent
		// forwardPacket() in satroute.cc (when wiredRouting_ == 0)
		ch->next_hop_ = next_hopIP;
		if (satnode()) {
			ch->last_hop_ = satnode()->ragent()->myaddr();
		} else {
			printf("Error:  LL has no satnode_ pointer set\n");
			exit(1);
		}
        }
	// else (if no wired rtg) next-hop field is populated by rtg agent
	
	return ch->next_hop_;
	
}


// Encode link layer sequence number, type, and mac address fields
void SatLL::sendDown(Packet* p)
{	
	hdr_cmn *ch = HDR_CMN(p);
	hdr_ll *llh = HDR_LL(p);
	
	char *mh = (char*)p->access(hdr_mac::offset_);
	int peer_mac_;
	SatChannel* satchannel_;

	llh->seqno_ = ++seqno_;
	llh->lltype() = LL_DATA;

	getRoute(p);
	
	// Set mac src, type, and dst
	mac_->hdr_src(mh, mac_->addr());
	mac_->hdr_type(mh, ETHERTYPE_IP); // We'll just use ETHERTYPE_IP
	
	nsaddr_t dst = ch->next_hop();
	// a value of -1 is IP_BROADCAST
	if (dst < -1) {
		printf("Error:  next_hop_ field not set by routing agent\n");
		exit(1);
	}

	switch(ch->addr_type()) {

	case NS_AF_INET:
	case NS_AF_NONE:
		if (IP_BROADCAST == (u_int32_t) dst)
			{
			mac_->hdr_dst((char*) HDR_MAC(p), MAC_BROADCAST);
			break;
		}
		/* 
		 * Here is where arp would normally occur.  In the satellite
		 * case, we don't arp (for now).  Instead, use destination
		 * address to find the mac address corresponding to the
		 * peer connected to this channel.  If someone wants to
		 * add arp, look at how the wireless code does it.
		 */ 
		// Cache latest value used
		if (dst == arpcachedst_) {
			mac_->hdr_dst((char*) HDR_MAC(p), arpcache_);
			break;
		}
		// Search for peer's mac address (this is the pseudo-ARP)
		satchannel_ = (SatChannel*) channel();
		peer_mac_ = satchannel_->find_peer_mac_addr(dst);
		if (peer_mac_ < 0 ) {
			printf("Error:  couldn't find dest mac on channel ");
			printf("for src/dst %d %d at NOW %f\n", 
			    ch->last_hop_, dst, NOW); 
			exit(1);
		} else {
			mac_->hdr_dst((char*) HDR_MAC(p), peer_mac_);
			arpcachedst_ = dst;
			arpcache_ = peer_mac_;
			break;
		} 

	default:
		printf("Error:  addr_type not set to NS_AF_INET or NS_AF_NONE\n");
		exit(1);
	}
	
	// let mac decide when to take a new packet from the queue.
	Scheduler& s = Scheduler::instance();
	s.schedule(downtarget_, p, delay_);
}

void SatLL::sendUp(Packet* p)
{
	Scheduler& s = Scheduler::instance();
	if (hdr_cmn::access(p)->error() > 0)
		drop(p);
	else
		s.schedule(uptarget_, p, delay_);
}

// Helper function 
Channel* SatLL::channel()
{
	Phy* phy_ = (Phy*) mac_->downtarget();
	return phy_->channel();
}

/*==========================================================================*/
/*
 * _SatMac
 */

static class SatMacClass : public TclClass {
public:
	SatMacClass() : TclClass("Mac/Sat") {}
	TclObject* create(int, const char*const*) {
		return (new SatMac);
	}
} sat_class_mac;

void MacSendTimer::expire(Event*)
{
        a_->send_timer();
}

void MacRecvTimer::expire(Event*)
{
        a_->recv_timer();
}

SatMac::SatMac() : Mac(), send_timer_(this), recv_timer_(this)
{
	bind_bool("trace_collisions_", &trace_collisions_);
	bind_bool("trace_drops_", &trace_drops_);
}

int SatMac::command(int argc, const char*const* argv)
{
	Tcl& tcl = Tcl::instance();
	if(argc == 2) {
	}
	else if (argc == 3) {
		TclObject *obj;
		if( (obj = TclObject::lookup(argv[2])) == 0) {
			fprintf(stderr, "%s lookup failed\n", argv[1]);
			return TCL_ERROR;                
		}
		if (strcmp(argv[1], "channel") == 0) {
			//channel_ = (Channel*) obj;
			return (TCL_OK);
		}
		if (strcmp(argv[1], "set_drop_trace") == 0) {
			drop_trace_ = (SatTrace *) TclObject::lookup(argv[2]);
			if (drop_trace_ == 0) {
				tcl.resultf("no such object %s", argv[2]);
				return (TCL_ERROR);
			}
			return (TCL_OK);
		}
		if (strcmp(argv[1], "set_coll_trace") == 0) {
			coll_trace_ = (SatTrace *) TclObject::lookup(argv[2]);
			if (coll_trace_ == 0) {
				tcl.resultf("no such object %s", argv[2]);
				return (TCL_ERROR);
			}
			return (TCL_OK);
		}
	}
	return Mac::command(argc, argv);
}

void SatMac::sendUp(Packet* p) 
{
	hdr_mac* mh = HDR_MAC(p);
	int dst = this->hdr_dst((char*)mh); // mac destination address
	
	if (((u_int32_t)dst != MAC_BROADCAST) && (dst != index_)) {
		drop(p);
		return;
	}
	// First bit of packet has arrived-- wait for 
	// (txtime + delay_) before sending up
	Scheduler::instance().schedule(uptarget_, p, delay_ + mh->txtime());
}



void SatMac::sendDown(Packet* p)
{
	Scheduler& s = Scheduler::instance();
	double txt;
	// LINK_HDRSIZE is defined in satlink.h.  This is the size of header
	// information for all layers below IP.  Alternatively, one could
	// derive this information dynamically from packet headers. 
	int packetsize_ = HDR_CMN(p)->size() + LINK_HDRSIZE;
	assert (bandwidth_ != 0);
	txt = txtime(packetsize_);
	// For convenience, we encode the transmit time in the Mac header
	// The packet will be held (for collision detection) for txtime 
	// at the receiving mac.
        HDR_MAC(p)->txtime() = txt;
	downtarget_->recv(p, this);
	// Callback for when this packet's transmission will be done
	s.schedule(&hRes_, &intr_, txt);
}


static class UnslottedAlohaMacClass : public TclClass {
public:
	UnslottedAlohaMacClass() : TclClass("Mac/Sat/UnslottedAloha") {}
	TclObject* create(int, const char*const*) {
		return (new UnslottedAlohaMac());
	}
} sat_class_unslottedalohamac;


//************************************************************************************************
//global parameters
//G
//bit_length
//int half_retrans_max=5;// max times of retransmission
//double K_retrans=0.5;//half retransmission
//double bit_duration=0.006;
//unsigned int  windowt=40;//length of sliding window
//unsigned int deltat=10;//shift interval of the sliding window
//int decode_iter_max=100;
//int buffer_max=2*windowt*256;//256-frame length

vector<Packet*> ZigZagDecode::zdecode(unsigned int start_bit_, unsigned int end_bit_){
		multimap<unsigned int, PacketBit*> rcv_bit_;
		for(multimap<unsigned int, PacketBit*>::iterator iter=t_->rcv_bit_buffer_.begin();iter!=t_->rcv_bit_buffer_.end();iter++){
			if((*iter).first>=start_bit_ && (*iter).first <= end_bit_){
				rcv_bit_.insert(*iter);
			}
		}
		//ZigZag Decoding
		int decode_iter_num=0;
		while(decode_iter_num<=t_->decode_iter_max){
			bool flag_=false;//Is there any bit_slot of which degree equals 1?
			for(unsigned int i=start_bit_;i<=end_bit_;i++){
				//i--current bit_slot

				if(rcv_bit_.count(i)==1){//decode from here
					flag_=true;// be able to decode

					pair<unsigned int, PacketBit*> p=*(rcv_bit_.equal_range(i).first);
					PacketBit* pb_=p.second;

					//take down the Resolved bit

					map<unsigned int, PacketNode*>::iterator pn_it=t_->pktlist_buffer_.find(pb_->pn->uid_);
					if(pn_it!=t_->pktlist_buffer_.end()){
					(*pn_it).second->Noded_bit_counter++;
					}
					// delete the bit in rcv_bit_
					for(vector<unsigned int>::iterator it=pb_->pn->location_pool.begin();it!=pb_->pn->location_pool.end();it++){
						//*it--corresponding bit_slot
						unsigned int location_=pb_->bit_id+(*it);

						if(location_!= i){
	                        pair<multimap<unsigned int, PacketBit*>::iterator,multimap<unsigned int, PacketBit*>::iterator> pitt=rcv_bit_.equal_range(location_);
	                        multimap<unsigned int, PacketBit*>::iterator itt=pitt.first;

						while(itt!=pitt.second){
	                        PacketBit* pb=(*itt).second;

							if((*itt).second->pn->uid_ == pb->pn->uid_ && (*itt).second->bit_id == pb->bit_id ){
								rcv_bit_.erase(itt);
								break;
							}
							itt++;
							if(itt==pitt.second){
								printf("Wrong Records of Bit Location!");
								exit(1);
							}
						}
						}
					}
					rcv_bit_.erase(i);
				}
			}

			if(!flag_ && !rcv_bit_.empty()){//rcv_bit_ is not empty and no bit_slot's degree=1 => decoding cannot be executed again
				//decoding stops
				break;
			}
			decode_iter_num++;
		}

		// check which packets that have been Noded
		vector<Packet*> ans;
		for(map<unsigned int, PacketNode*>::iterator iter=t_->pktlist_buffer_.begin();iter!=t_->pktlist_buffer_.end();iter++){
			if((*iter).second->Noded_bit_counter==(*iter).second->dcsize){
				// successfully decoded
				Packet* p=(*iter).second->p;
				ans.push_back(p);
			}
		}
		return ans;
		}

void AlohaDecodeTimer::expire(Event* e){
	cout<<"Event uid"<<e->uid_<<endl;
	///zigzag decoding process;
	unsigned int end_bit_=floor(NOW/(t_->bit_duration));
	unsigned int start_bit_=end_bit_-t_->windowt+1;

	vector<Packet*> ans=t_->zdecoder_.zdecode(start_bit_,end_bit_);
//	t_->rx_state_ = MAC_RECV;

//	for(vector<Packet*>::iterator iter=ans.begin();iter!=ans.end();iter++){
//		Packet* p=*iter;
//		//remove p from buffer_,pktlist_buffer_,rcv_bit_buffer_
//		t_->end_of_contention(p);
//		}

	if(!ans.empty()) t_->sendupZDPackets(ans);
//	cout<<e->uid_<<endl;

	////////////////////////////////////////////////////////////////////////////////////////////

	t_->decode_timer_.resched(t_->deltat*t_->bit_duration);
}

void LwrTimer::expire(Event* e){
	t_->snd_pkt_->refcopy();
	if(Random::uniform()<t_->K_retrans){//
		t_->snd_pkt_->halfRetrans(0.5);
	}else{
		t_->snd_pkt_->fullRetrans();
	}
	Packet* p=t_->snd_pkt_->copy();
	t_->downtarget_->recv(p, this);
}

/*==========================================================================*/
/*
 * _UnslottedAlohaMac
 */

UnslottedAlohaMac::UnslottedAlohaMac() : SatMac(), tx_state_(MAC_IDLE),
    rx_state_(MAC_IDLE), rtx_(0), end_of_contention_(0),arriv_time_(0), buffered_bit_num_(0),
    decode_timer_(this),zdecoder_(this),timer_on_(false) ,lwr_timer_(this)
{
	bind_time("mean_backoff_", &mean_backoff_);
	bind("rtx_limit_", &rtx_limit_);
	bind_time("send_timeout_", &send_timeout_);
	
        // new configuration
        bind("half_retrans_max", &half_retrans_max);
        bind("K_retrans",&K_retrans);
        bind_time("bit_duration",&bit_duration);
        bind("windowt",&windowt);
        bind("deltat",&deltat);
        bind("decode_iter_max",&decode_iter_max);
        bind("buffer_max",&buffer_max);
        //decode_timer_.sched(bit_duration*windowt);

 }


void UnslottedAlohaMac::insertPacket(Packet* p,unsigned int bit_slot){

//inserted into buffer_

	if(buffer_.size()>=buffer_max){
		this->releaseBuffer();
	}

	buffer_.insert(pair<unsigned int,Packet*>(bit_slot,p));

	PacketNode* pn=new PacketNode;
	hdr_cmn* ch=HDR_CMN(p);
	int uid_=ch->uid_;


//inserted into pktlist
	if(pktlist_buffer_.find(uid_)==pktlist_buffer_.end() ){
		pn->p=p;
		pn->uid_=uid_;
		pn->dcsize=p->getDcsize();
		pn->Noded_bit_counter=0;
		pn->location_pool.push_back(bit_slot);
		pktlist_buffer_.insert(pair<int, PacketNode*>(uid_,pn));
		(*pktlist_buffer_.find(uid_)).second->location_pool.push_back(bit_slot);
		pktlist_buffer_.insert(pair<unsigned int, PacketNode*>(bit_slot,pn));
	}else{
		(*pktlist_buffer_.find(uid_)).second->location_pool.push_back(bit_slot);
			}
//inserted into rcv_bits
	for(int j=0;j<=p->getReDcsize()-1;j++){
		PacketBit* pb = new PacketBit;
		pb->pn=pn;
		pb->bit_id=j;
		rcv_bit_buffer_.insert(pair<unsigned int,PacketBit*> (bit_slot+j,pb));
	}

}

void UnslottedAlohaMac::sendupZDPackets(vector<Packet*> &vec){
	if(vec.empty()) return;

//	rx_state_ = MAC_IDLE;
	for(vector<Packet*>::iterator iter=vec.begin();iter!=vec.end();iter++){
		Packet* p=*iter;

		hdr_cmn* ch=hdr_cmn::access(p);
		int uid_=ch->uid_;
		map<unsigned int, PacketNode*>::iterator pn_it=pktlist_buffer_.equal_range(uid_).first;
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
				rcv_bit_buffer_.erase(curr_it);
				curr_it=next_it;
				next_it++;
			}else{
				curr_it++;
				next_it++;
			}
		}

		//remove from pktlist_buffer_
		map<unsigned int, PacketNode*>::iterator pb_it=pktlist_buffer_.equal_range(uid_).first;
		delete (*pb_it).second;
		pktlist_buffer_.erase(uid_);

		//remove from buffer_
		for(vector<unsigned int>::iterator lp_it=location_pool_.begin();lp_it!=location_pool_.end();lp_it++){
			multimap<unsigned int, Packet*>::iterator bf1=buffer_.equal_range(*lp_it).first;
			multimap<unsigned int, Packet*>::iterator bf2=buffer_.equal_range(*lp_it).second;
			for(multimap<unsigned int, Packet*>::iterator bf_it=bf1;bf_it!=bf2;bf_it++){
				if((*bf_it).second == p){
					buffer_.erase(bf_it);
					break;
				}
				}
			}

//		end_of_contention(p);

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
				send_timer_.force_cancel();
				tx_state_ = MAC_IDLE;
				rtx_ = 0;
				drop(snd_pkt_); // Free the packet cached for retransmission
				resume(p);
			} else {
				// wait for processing delay (delay_) to send packet upwards
				Scheduler::instance().schedule(uptarget_, p, delay_);
			}
		}

	}


void UnslottedAlohaMac::releaseBuffer(){
	multimap<unsigned int, Packet*>::iterator b_iter=buffer_.begin();
	for(int i=0;i<=buffer_max/2-1;i++)b_iter++;
	unsigned int key_=(*(b_iter)).first;

	//release rcv_bit_buffer_
	multimap<unsigned int,PacketBit*>::iterator biter = rcv_bit_buffer_.begin();
	while(unsigned int i=(*biter).first<=key_){
		pair<multimap<unsigned int,PacketBit*>::iterator,multimap<unsigned int,PacketBit*>::iterator> piter=rcv_bit_buffer_.equal_range(i);
		for(multimap<unsigned int,PacketBit*>::iterator iter =piter.first ;iter!=piter.second;iter++){
				PacketBit* pb=(*iter).second;
				delete pb;
		}
		biter=piter.second;
		 rcv_bit_buffer_.erase(i);
		}

	//release pktlist
	map<unsigned int,PacketNode*>::iterator niter = pktlist_buffer_.begin();
	while(unsigned int i=(*niter).first<=key_){
		PacketNode* pn=(*niter).second;
		if(pn->location_pool.size()==1 && pn->location_pool[0]==i ) {
			delete pn;
			niter++;
			pktlist_buffer_.erase(i);
		}else if(pn->location_pool.size()==1 && pn->location_pool[0]==i) assert("wrong location!");
		else {
			vector<unsigned int>::iterator erase_it;
			for(vector<unsigned int>::iterator it=pn->location_pool.begin();it!=pn->location_pool.end();it++){
				if(*it == i) erase_it=it;
			}
			pn->location_pool.erase(erase_it);
			niter++;
		}
	}

	//release buffer_
	unsigned int bf_head_=(*(buffer_.begin())).first;
	for(unsigned int i=bf_head_;i<=key_;i++){
		if(buffer_.count(i)!=0){
			multimap<unsigned int, Packet*>::iterator biter=(buffer_.equal_range(i)).first;
			multimap<unsigned int, Packet*>::iterator eiter=(buffer_.equal_range(i)).second;
			for(multimap<unsigned int, Packet*>::iterator iter=biter;iter!=eiter;iter++){
				drop((*iter).second);
			}
			buffer_.erase(i);
		}
	}
}
///************************************************************************************************

void UnslottedAlohaMac::send_timer() 
{	
	switch (tx_state_) {
	
	case MAC_SEND:
		// We've timed out on send-- back off
		// means that last transmission failed, so having to backoff and retransmitting
		backoff();
		break;
	case MAC_COLL:
		// Our backoff timer has expired-- begin resending
//************************************************************************************************

		if(snd_pkt_->getRetansCounter() >= half_retrans_max){
			snd_pkt_->setHalfRetransCounter(0);
			snd_pkt_->originRetrans();
		}else{
			if(Random::uniform()<K_retrans){//
				snd_pkt_->halfRetrans(0.5);
			}else{
				snd_pkt_->fullRetrans();
			}
		}
///************************************************************************************************
		sendDown(snd_pkt_);
		break;
	default:
		printf("Error: wrong tx_state in unslotted aloha: %d\n",
		    tx_state_);
		break;
	}
}

void UnslottedAlohaMac::recv_timer() 
{
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
	
}

void UnslottedAlohaMac::sendUp(Packet* p) 
{
	hdr_cmn* ch=hdr_cmn::access(p);

	hdr_mac* mh = HDR_MAC(p);

	int dst;

	if (rx_state_ == MAC_IDLE) {
		// First bit of packet has arrived-- wait for 
		// txtime to make sure no collisions occur 
		//the packet can be received successfully
		rcv_pkt_ = p;
		end_of_contention_ = NOW + mh->txtime();//the time of current packet finishing transmission
		rx_state_ = MAC_RECV;
		arriv_time_=NOW;
		recv_timer_.resched(mh->txtime());
	} else {
		// Collision: figure out if contention phase must be lengthened
		if ( (NOW + mh->txtime()) > end_of_contention_ ) {
			recv_timer_.resched(mh->txtime());//if this recv_timer_ is set, last rcv_timer_ will be cancelled
											// so the contention phase is lengthened
		}
		//*************************************************************************************
		// buffer the packet

			unsigned int key_=floor(NOW/bit_duration);
//			while(buffer_.size()>buffer_max){
//				unsigned int key_min=(*buffer_.begin()).first;
//				buffer_.erase(key_min);
//			}

			//

			if(rcv_pkt_){
				this->insertPacket(rcv_pkt_,floor(arriv_time_/bit_duration));
				rcv_pkt_ = 0;
				arriv_time_=0;
			}

			this->insertPacket(p,key_);
			if(!timer_on_){
			 decode_timer_.sched(0.0009);
			 timer_on_=true;
			}
	}
}

void UnslottedAlohaMac::sendDown(Packet* p)
{
	double txt;
	// compute transmission delay:
	int packetsize_ = HDR_CMN(p)->size() + LINK_HDRSIZE;
	assert (bandwidth_ != 0);
	txt = txtime(packetsize_);
        HDR_MAC(p)->txtime() = txt;
        cout<<"txt= "<<txt<<endl;
	// Send the packet down 
	tx_state_ = MAC_SEND;
	snd_pkt_ = p->copy();  // save a copy in case it gets retransmitted
//	snd_pkt_->refcopy();
	downtarget_->recv(p, this);

	// Set a timer-- if we do not hear our own transmission within this
	// interval (and cancel the timer), the send_timer will expire and
	// we will backoff and retransmit.
	if(snd_pkt_->getRetansCounter()==0){
		int n= Random::integer(snd_pkt_->getDcsize());
		lwr_timer_.sched(n*bit_duration+txt);
	}
		send_timer_.resched(send_timeout_ + txt);

}

// Called when contention period ends
void UnslottedAlohaMac::end_of_contention(Packet* p) 
{
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
			exit(1);
		}
		send_timer_.force_cancel();
		tx_state_ = MAC_IDLE;
		rtx_ = 0;
		drop(snd_pkt_); // Free the packet cached for retransmission
		resume(p);
	} else {
		// wait for processing delay (delay_) to send packet upwards 
		Scheduler::instance().schedule(uptarget_, p, delay_);
	}
}

void UnslottedAlohaMac::backoff(double delay)
{
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
}


/*==========================================================================*/
/*
 * _SatPhy
 */

static class SatPhyClass: public TclClass {
public:
	SatPhyClass() : TclClass("Phy/Sat") {}
	TclObject* create(int, const char*const*) {
		return (new SatPhy);
	}
} class_SatPhy;

void SatPhy::sendDown(Packet *p)
{
	if (channel_)
		channel_->recv(p, this);
	else {
		// it is possible for routing to change (and a channel to
		// be disconnected) while a packet
		// is moving down the stack.  Therefore, just log a drop
		// if there is no channel
		if ( ((SatNode*) head()->node())->trace() )
			((SatNode*) head()->node())->trace()->traceonly(p);
		Packet::free(p);
	}
}

// Note that this doesn't do that much right now.  If you want to incorporate
// an error model, you could insert a "propagation" object like in the
// wireless case.
int SatPhy::sendUp(Packet * /* pkt */)
{
	return TRUE;
}

int
SatPhy::command(int argc, const char*const* argv) {
	if (argc == 2) {
	} else if (argc == 3) {
		TclObject *obj;

		if( (obj = TclObject::lookup(argv[2])) == 0) {
			fprintf(stderr, "%s lookup failed\n", argv[1]);
			return TCL_ERROR;
		}
	}
	return Phy::command(argc, argv);
}

static class RepeaterPhyClass: public TclClass {
public:
	RepeaterPhyClass() : TclClass("Phy/Repeater") {}
	TclObject* create(int, const char*const*) {
		return (new RepeaterPhy);
	}
} class_RepeaterPhy;

void RepeaterPhy::recv(Packet* p, Handler*)
{
	struct hdr_cmn *hdr = HDR_CMN(p);
	if (hdr->direction() == hdr_cmn::UP) {
		// change direction and send to uptarget (which is
		// really a Phy_tx that is also a RepeaterPhy)
		hdr->direction() = hdr_cmn::DOWN;
		uptarget_->recv(p, (Handler*) 0);
	} else {
		sendDown(p);
	}
}

void RepeaterPhy::sendDown(Packet *p)
{
	struct hdr_cmn *hdr = HDR_CMN(p);
	hdr->direction() =  hdr_cmn::DOWN;

	if (channel_)
		channel_->recv(p, this);
	else {
		printf("Error, no channel on repeater\n");
		exit(1);
	}
}

/*==========================================================================*/
/*
 * _SatChannel
 */

static class SatChannelClass : public TclClass {
public:
	SatChannelClass() : TclClass("Channel/Sat") {}
	TclObject* create(int, const char*const*) {
		return (new SatChannel);
	}
} class_Sat_channel;

SatChannel::SatChannel(void) : Channel() {
}

double
SatChannel::get_pdelay(Node* tnode, Node* rnode)
{
	coordinate a = ((SatNode*)tnode)->position()->coord();
	coordinate b = ((SatNode*)rnode)->position()->coord();
	return (SatGeometry::propdelay(a, b));
}

// This is a helper function that attaches a SatChannel to a Phy
void SatChannel::add_interface(Phy* phy_)
{
	phy_->setchnl(this); // Attach phy to this channel
	phy_->insertchnl(&ifhead_); // Add phy_ to list of phys on the channel
}

// Remove a phy from a channel
void SatChannel::remove_interface(Phy* phy_)
{
	phy_->setchnl(NULL); // Set phy_'s channel pointer to NULL
	phy_->removechnl(); // Remove phy_ to list of phys on the channel
}

// Search for destination mac address on this channel.  Look through list
// of phys on the channel.  If the channel connects to a geo repeater, look
// for the destination on the corresponding downlink channel.  
int SatChannel::find_peer_mac_addr(int dst)
{
	Phy *n;
	Channel* chan_;
	chan_ = this;
	n = ifhead_.lh_first; 
	if (n->head()->type() == LINK_GSL_REPEATER) {
		SatLinkHead* slh = (SatLinkHead*) n->head();
		chan_ = slh->phy_tx()->channel();
	}
	for(n = chan_->ifhead_.lh_first; n; n = n->nextchnl() ) {
		if (n->node()->address() == dst) {
			return (((SatMac*) n->uptarget())->addr());
		}
	}
	return -1;
}


