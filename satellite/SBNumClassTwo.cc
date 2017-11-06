#include "SBNumClassTwo.h"

static class SBNumClassTwoClass : public TclClass
{
	public:
	SBNumClassTwoClass() : TclClass("SBNumClassTwo") {}
		TclObject *create ( int, const char* const* ){
			return (new SBNumClassTwo ());
		}
} class_sbtwoobject;

int SBNumClassTwo:: command(int argc, const char* const* argv)
{
 	Tcl&tcl = Tcl :: instance();
	if (strcmp(argv[1], "returnSBNumClassTwo") == 0) {
		tcl.resultf("%d",sb_of_class_two);
	}
	if (strcmp(argv[1], "returnPktNumClassTwo") == 0) {
			tcl.resultf("%d",reveal_pkt_two.size());

		}
	return TCL_OK;
}

int SBNumClassTwo:: sb_of_class_two = 0;
vector<int> SBNumClassTwo::reveal_pkt_two = vector<int> ();
