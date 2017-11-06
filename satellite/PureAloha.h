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
 * @(#) $Header: /cvsroot/nsnam/ns-2/satellite/satlink.h,v 1.7 2005/09/21 20:52:47 haldar Exp $
 *
 * Contributed by Tom Henderson, UCB Daedalus Research Group, June 1999
 */

#ifndef PureAloha_h
#define PureALoha_h

# include "satlink.h"
#include "random.h"
#include "node.h"
#include "channel.h"
#include "phy.h"
#include "queue.h"
#include "net-interface.h"
#include "timer-handler.h"

class SatLinkHead;
class SatTrace;
class SatNode;

class PureAloha : public SatMac {
public:
	PureAloha();
	void sendDown(Packet* p); 
	void sendUp(Packet *p); 
	void send_timer(); 
	void recv_timer(); 
	void end_of_contention(Packet* p);

protected:
	virtual void backoff(double delay=0);
	Packet* snd_pkt_;	// stores packet currently being sent
	Packet* rcv_pkt_;	// stores packet currently being recieved
	MacState tx_state_;	// transmit state (SEND or COLL or IDLE)
	MacState rx_state_;	// receive state (RECV or IDLE)
	int rtx_; 		// # of retransmissions so far
	int rtx_limit_;		// Set in OTcl-- retransmission limit
	double mean_backoff_;	// Set in OTcl-- mean backoff time 
	double send_timeout_;	// Set in OTcl-- time out after this interval
	double end_of_contention_; // Saves time that contention will be over

};

///////////////////////////////////////////////////////////////////////



#endif
