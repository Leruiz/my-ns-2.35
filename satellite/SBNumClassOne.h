#include "satlink.h"
#include "node.h"
#include "channel.h"
#include "phy.h"
#include "queue.h"
#include "net-interface.h"
#include "timer-handler.h"
#include <set>
#include <vector>
#include <map>

class SBNumClassOne :  public TclObject
{
public :
	static int sb_of_class_one ;
	static int sb_decoded;
	static vector<int> reveal_pkt_one;
	static map<int, vector<bool> > decosb_;
 	SBNumClassOne() {}
	int command(int, const char* const* );
};
