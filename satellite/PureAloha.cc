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

#include "PureAloha.h"
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




static class PureAlohaClass : public TclClass {
public:
	PureAlohaClass() : TclClass("Mac/Sat/PureAloha") {}
	TclObject* create(int, const char*const*) {
		return (new PureAloha());
	}
} sat_class_purealohamac;
/*==========================================================================*/
/*
 * _PureAloha
 */

PureAloha::PureAloha() : SatMac(), tx_state_(MAC_IDLE),
    rx_state_(MAC_IDLE), rtx_(0), end_of_contention_(0)
{
	bind_time("mean_backoff_", &mean_backoff_);
	bind("rtx_limit_", &rtx_limit_);
	bind_time("send_timeout_", &send_timeout_);
}

void PureAloha::send_timer()
{
	hdr_cmn* ch=hdr_cmn::access(snd_pkt_);
	cout<<"In send_timer   index_: "<<index_<<"  pkt uid_: "<<ch->uid_<<"  Now: "<<NOW<<endl;
	switch (tx_state_) {
	
	case MAC_SEND:
		// We've timed out on send-- back off
		backoff();
		break;
	case MAC_COLL:
		// Our backoff timer has expired-- resend
		sendDown(snd_pkt_);
		break;
	default:
		printf("Error: wrong tx_state in unslotted aloha: %d\n",
		    tx_state_);
		break;
	}
}

void PureAloha::recv_timer()
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

void PureAloha::sendUp(Packet* p)
{

	hdr_mac* mh = HDR_MAC(p);
	int dst;
	
	if (rx_state_ == MAC_IDLE) {
		// First bit of packet has arrived-- wait for 
		// txtime to make sure no collisions occur 
		rcv_pkt_ = p;
		end_of_contention_ = NOW + mh->txtime();
		rx_state_ = MAC_RECV;
		recv_timer_.resched(mh->txtime());
	} else {
		// Collision: figure out if contention phase must be lengthened
		if ( (NOW + mh->txtime()) > end_of_contention_ ) {
			recv_timer_.resched(mh->txtime());
		}
		// If this is the first collision, we will also have a
		// rcv_pkt_ pending
		if (rcv_pkt_) {
			// Before dropping rcv_pkt_, trace the collision
			// if it was intended for us
			mh = HDR_MAC(rcv_pkt_);
			dst = this->hdr_dst((char*)mh); // mac dest. address
			if (((u_int32_t)dst == MAC_BROADCAST)||(dst == index_))
				if (coll_trace_ && trace_collisions_)
					coll_trace_->traceonly(rcv_pkt_);
			drop(rcv_pkt_);
		}
		rcv_pkt_ = 0;
		// Again, before we drop this packet, log a collision if
		// it was intended for us
		mh = HDR_MAC(p);
		dst = this->hdr_dst((char*)mh); // mac destination address
		if (((u_int32_t)dst == MAC_BROADCAST) || (dst == index_))
			if (coll_trace_ && trace_collisions_)
				coll_trace_->traceonly(p);
		drop(p);
	}
}

static int num_ref=0;
void PureAloha::sendDown(Packet* p)
{
	if(p->getRetansCounter()==0){
		hdr_cmn* ch=hdr_cmn::access(p);
		cout<<"index_: "<<index_<<"  pkt uid_: "<<ch->uid_<<"  Now: "<<NOW<<endl;
	}
	double txt;
	
	// compute transmission delay:
	int packetsize_ = HDR_CMN(p)->size() + LINK_HDRSIZE;
	assert (bandwidth_ != 0);
	txt = txtime(packetsize_);
        HDR_MAC(p)->txtime() = txt;


	// Send the packet down 
	tx_state_ = MAC_SEND;
	snd_pkt_ = p->copy();  // save a copy in case it gets retransmitted
	snd_pkt_->refcopy();
	num_ref++;
	cout<<"num_ref="<<num_ref<<endl;

	downtarget_->recv(p, this);

	// Set a timer-- if we do not hear our own transmission within this
	// interval (and cancel the timer), the send_timer will expire and
	// we will backoff and retransmit.

	send_timer_.resched(send_timeout_ + txt);
}

// Called when contention period ends
void PureAloha::end_of_contention(Packet* p)
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

void PureAloha::backoff(double delay)
{
	double backoff_ = Random::exponential(mean_backoff_);

	// if number of retransmissions is within limit, do exponential backoff
	// else drop the packet and resume
	if (++rtx_ <= rtx_limit_) {
		tx_state_ = MAC_COLL;
		delay += backoff_;
		send_timer_.resched(delay);
	} else {
		tx_state_ = MAC_IDLE;
		rtx_ = 0;
		// trace the dropped packet
		if (drop_trace_ && trace_drops_)
			drop_trace_->traceonly(snd_pkt_);
		resume(snd_pkt_);
	}
}




