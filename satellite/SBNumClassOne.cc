#include "SBNumClassOne.h"

static class SBNumClassOneClass : public TclClass
{
	public:
	SBNumClassOneClass() : TclClass("SBNumClassOne") {}
		TclObject *create ( int, const char* const* ){
			return (new SBNumClassOne ());
		}
} class_sboneobject;

int SBNumClassOne:: command(int argc, const char* const* argv)
{
 	Tcl&tcl = Tcl :: instance();
	if (strcmp(argv[1], "returnSBNumClassOne") == 0) {
		tcl.resultf("%d",sb_of_class_one);

	}else if (strcmp(argv[1], "returnPktNumClassOne") == 0) {
			tcl.resultf("%d",reveal_pkt_one.size());

		}else if (strcmp(argv[1], "returnSBNumDecoded") == 0){
			tcl.resultf("%d",sb_decoded);
		}
	return TCL_OK;
}

int SBNumClassOne:: sb_of_class_one = 0;
int SBNumClassOne::sb_decoded = 0;
vector<int> SBNumClassOne::reveal_pkt_one = vector<int> ();
map<int, vector<bool> > SBNumClassOne::decosb_ = map<int, vector<bool> > ();

