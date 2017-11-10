#include "RetransTimes.h"

static class RetransTimesClass : public TclClass
{
public:
	RetransTimesClass() : TclClass ("RetransTimes") {}
	TclObject *create (int, const char * const*)
	{
		return ( new RetransTimes() );
	}
}class_retranstimesobj;

int RetransTimes::command(int argc, const char* const* argv)
{
	Tcl&tcl = Tcl :: instance();
	if (strcmp(argv[1], "returnRetransTimeSum") == 0)
	{
		tcl.resultf("%d",success_retrans_times_sum_);

	}else if(strcmp(argv[1], "returnPracSentPkt") == 0)
	{
		tcl.resultf("%d", prac_sent_pkt_);
	}
	return TCL_OK;
}


int RetransTimes:: success_retrans_times_sum_ = 0;
int RetransTimes::success_pkt_num_ = 0;
int RetransTimes::prac_sent_pkt_ = 0;


