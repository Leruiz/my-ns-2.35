#include "satlink.h"
#include "node.h"
#include "channel.h"
#include "phy.h"
#include "queue.h"
#include "net-interface.h"
#include "timer-handler.h"
#include <set>
#include <vector>

class SBNumClassTwo :  public TclObject
{
public :
	static int sb_of_class_two ;
	static vector<int> reveal_pkt_two;
 	SBNumClassTwo() {}
	int command(int, const char* const* );
};
