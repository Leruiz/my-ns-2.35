#include "satlink.h"
#include "node.h"
#include "channel.h"
#include "phy.h"
#include "queue.h"
#include "net-interface.h"
#include "timer-handler.h"
#include <map>

class RetransTimes : public TclObject
{
public:
	static int success_retrans_times_sum_;
	static int success_pkt_num_;
	static double ave_retrans_times_;
	static int k_;
	static int prac_sent_pkt_;

	RetransTimes() {}
	int command (int, const char* const*);
};

