/*
 * CRDSA.h
 *
 *  Created on: Nov 2, 2016
 *      Author: jing
 */

#ifndef CRDSA_H_
#define CRDSA_H_

#include "satlink.h"

class CRDSA;
class SICTimer:public TimerHandler{
protected:
	CRDSA * t_;
public:
	SICTimer (CRDSA* t):TimerHandler(),t_(t){}
	virtual void expire(Event* e);
};


class CRDSA : public SatMac {
public:
	CRDSA();
	void sendDown(Packet* p);
	void sendUp(Packet *p);
//	void send_timer();
//	void recv_timer();
//	void end_of_contention(Packet* p);
	void SIC();



protected:
	friend class SICTimer;

	SICTimer sic_;

	int frame_length_;
	double slot_duration_;
	multimap<int, Packet*> buffer_;
	bool timeron;
};

#endif /* CRDSA_H_ */
