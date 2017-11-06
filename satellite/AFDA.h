	//#ifndef AFDA_h
	//#define AFDA_h

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

	class AFDA;

	class ADecodeTimer: public TimerHandler{
	protected:
		AFDA *t_;
	public:
		ADecodeTimer (AFDA *t): TimerHandler(),t_(t){}
		virtual void expire(Event* e);
	};

	class ALwrTimer: public TimerHandler{
	protected:
		AFDA* t_;
	public:
		ALwrTimer(AFDA* t):TimerHandler(),t_(t){}
		virtual void expire(Event* e);
	};

	class ADecode {
	public:
		AFDA* t_;

		ADecode(AFDA* t):t_(t){}
		//~ADecode(){delete t_;}

		vector<Packet*> adecode(unsigned int , unsigned int );
	};

	///********************************************************************************************************************************

	class AFDA : public SatMac {
	public:
		AFDA();
		~AFDA();
		void sendDown(Packet* p);
		void sendUp(Packet *p);
		void send_timer();
		void recv_timer();
		void end_of_contention(Packet* );

		//added by Jing
		void insertPacket(Packet* ,unsigned int);
		void sendupAFDAckets(vector<Packet*> & );
		void releaseBuffer();
	//	void  ctrPktlsRcvBit(unsigned int ,unsigned int);

	protected:
	//*************************************************************************************
		friend class ADecodeTimer;
		friend class ADecode;
		friend class ALwrTimer;

		ADecodeTimer decode_timer_;
		ADecode adecoder_;
		ALwrTimer lwr_timer_;
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
	//	string construction; // Truncated , Random
		int decode_iter_max;//=100;
		int buffer_max;//=2*windowt*256;//256-frame length
		 static set<int> succ_pkt_set_;
		map<unsigned int ,set<int> > temp_pktlist_;
};

