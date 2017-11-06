#include "SolitonAveLen.h"

static class SolitonAveLenClass : public TclClass
{
public:
	SolitonAveLenClass() : TclClass("SolitonAveLen") { }
	TclObject *create ( int, const char* const* ){
		return (new SolitonAveLen ());
	}
} class_solitonavelenobj;

int SolitonAveLen :: command(int argc, const char* const* argv)
	{
		Tcl& tcl = Tcl :: instance();
		if(strcmp(argv[1], "returnSolitonAveLen") == 0)
		{
			if(rep_num_ != 0)
				soliton_ave_len_ = sum_len_ / rep_num_;
			tcl.resultf("%d", soliton_ave_len_);
		}
		return TCL_OK;
	}

int SolitonAveLen::sum_len_ = 0;
int SolitonAveLen::rep_num_ = 0;
int SolitonAveLen::soliton_ave_len_ = 0;

