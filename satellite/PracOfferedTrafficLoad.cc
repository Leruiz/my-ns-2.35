#include "PracOfferedTrafficLoad.h"

static class PracOfferedTrafficLoadClass : public TclClass 
{
	public:
		PracOfferedTrafficLoadClass() : TclClass("PracOfferedTrafficLoad") {}
		TclObject *create ( int, const char* const* ){
			return (new PracOfferedTrafficLoad ());		
		}
} class_pracotlobject;

int PracOfferedTrafficLoad:: command(int argc, const char* const* argv)
{
 	Tcl&tcl = Tcl :: instance();
	if (strcmp(argv[1], "returnPracLoad") == 0) {
		tcl.resultf("%d",num_sent_bits_);
	}
	return TCL_OK;
}
                     
int PracOfferedTrafficLoad:: num_sent_bits_ = 0;
