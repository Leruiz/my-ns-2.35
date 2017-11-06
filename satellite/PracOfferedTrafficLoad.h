//#ifndef ZDPA_h
	//#define ZDPA_h

	#include "satlink.h"
	#include "node.h"
	#include "channel.h"
	#include "phy.h"
	#include "queue.h"
	#include "net-interface.h"
	#include "timer-handler.h"

class PracOfferedTrafficLoad :  public TclObject
    {
            public :

                      static int num_sent_bits_ ;
                     PracOfferedTrafficLoad() {}
                     int command(int, const char* const* );
              
     };
