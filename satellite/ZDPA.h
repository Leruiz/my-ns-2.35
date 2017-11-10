	//#ifndef ZDPA_h
	//#define ZDPA_h

	#include "satlink.h"
	#include "node.h"
	#include "channel.h"
	#include "phy.h"
	#include "queue.h"
	#include "net-interface.h"
	#include "timer-handler.h"
	#include "PracOfferedTrafficLoad.h"
	#include "SBNumClassOne.h"
	#include "SBNumClassTwo.h"
	#include <map>
	#include <set>
	#include <vector>
	#include "RetransTimes.h"
	using namespace std;

	class ZDPA;

	class ZDecodeTimer: public TimerHandler{
	protected:
		ZDPA *t_;
	public:
		ZDecodeTimer (ZDPA *t): TimerHandler(),t_(t){}
		virtual void expire(Event* e);
	};

	class ZLwrTimer: public TimerHandler{
	protected:
		ZDPA* t_;
	public:
		ZLwrTimer(ZDPA* t):TimerHandler(),t_(t){}
		virtual void expire(Event* e);
	};

	class ZDecode {
	public:
		ZDPA* t_;

		ZDecode(ZDPA* t):t_(t){}
		//~ZDecode(){delete t_;}

		vector<Packet*> zdecode(unsigned int , unsigned int );
	};

	///********************************************************************************************************************************

	class ZDPA : public SatMac {
	public:
		ZDPA();
		~ZDPA();
		void sendDown(Packet* p);
		void sendUp(Packet *p);
		void send_timer();
		void recv_timer();
		void end_of_contention(Packet* );

		//added by Jing
		void insertPacket(Packet* ,unsigned int);
		void sendupZDPackets(vector<Packet*> & );
		void releaseBuffer();
	//	void  ctrPktlsRcvBit(unsigned int ,unsigned int);

	protected:
	//*************************************************************************************
		friend class ZDecodeTimer;
		friend class ZDecode;
		friend class ZLwrTimer;

		ZDecodeTimer decode_timer_;
		ZDecode zdecoder_;
		ZLwrTimer lwr_timer_;
	//	multimap<unsigned int,Packet*> buffer_;

		multimap<unsigned int,PacketBit*> rcv_bit_buffer_;
		map<unsigned int, PacketNode*> pktlist_buffer_;
		int buffered_bit_num_;
		bool timer_on_;
		int buffer_size_;

	///*************************************************************************************
		virtual void backoff(double delay=0);
		Packet* snd_pkt_;	// stores packet currently being sent
		Packet* rcv_pkt_;	// stores packet currently being recieved
		Packet* acc_pkt_;
		MacState tx_state_;	// transmit state (SEND or COLL or IDLE)
		MacState rx_state_;	// receive state (RECV or IDLE)
		int rtx_; 		// # of retransmissions so far
		int rtx_limit_;		// Set in OTcl-- retransmission limit
		double mean_backoff_;	// Set in OTcl-- mean backoff time
		double send_timeout_;	// Set in OTcl-- time out after this interval
		double end_of_contention_; // Saves time that contention will be over

		//int half_retrans_max;//=5;// max times of retransmission
		double bit_duration;//=0.006;
		unsigned int  windowt;//=40;//length of sliding window
		unsigned int deltat;//=10;//shift interval of the sliding window
		unsigned int optimal_deltat;
	//	string construction; // Truncated , Random
		int decode_iter_max;//=100;
		int buffer_max;//=2*windowt*256;//256-frame length
		 static set<int> succ_pkt_set_;
		map<unsigned int ,set<int> > temp_pktlist_;
		int k; //number of transmissions in a round
		double prob; //probability of partial retransmitting 
		double par_ratio; //ratio of partial packet
		 int construction;// = 0: trunc; =1:random; 2 = DRA
};

