
#include "satlink.h"
#include "node.h"
#include "channel.h"
#include "phy.h"
#include "queue.h"
#include "net-interface.h"
#include "timer-handler.h"
class SolitonAveLen :  public TclObject
{
public:
	static int soliton_ave_len_;
	static int rep_num_;
	static int sum_len_;
	SolitonAveLen(){};
	int command(int argc, const char* const* argv);
};

