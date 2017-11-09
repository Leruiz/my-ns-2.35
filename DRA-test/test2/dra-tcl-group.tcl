
if { $argc != 4} {
        puts stderr {4 argues : Poisson,  k, num_nodes, idle_time}
        exit 1
}
 
set test_ [lindex $argv 0]
set k_ [lindex $argv 1]
#set r_ [lindex $argv 3]
#set p_ [lindex $argv 4]
set n_node_ [lindex $argv 2]
set t_idle_ [lindex $argv 3]
puts "Running test $test_ ..."

global ns
set ns [new Simulator]



# Global configuration parameters for Aloha (also settable in ns-sat.tcl)
Mac/Sat/ZDPA set mean_backoff_ 0.01s ; # mean exponential backoff time(s)
Mac/Sat/ZDPA set rtx_limit_ 40; # max number of retrans. attempted 
Mac/Sat/ZDPA set send_timeout_ 4000ms; # resend if send times out

#Mac/Sat/ZDPA set half_retrans_max 3;
Mac/Sat/ZDPA set construction 2; #DRA protocol
Mac/Sat/ZDPA set k $k_;
Mac/Sat/ZDPA set prob 1;
Mac/Sat/ZDPA set par_ratio 1;
Mac/Sat/ZDPA set bit_duration 0.1ms;
Mac/Sat/ZDPA set windowt 20000 ;
Mac/Sat/ZDPA set deltat 10000 ;
Mac/Sat/ZDPA set decode_iter_max 5000;
Mac/Sat/ZDPA set buffer_max 5000;

if { $test_ == "basic"} {
	Mac/Sat set trace_collisions_ false
	Mac/Sat set trace_drops_ false
}

global opt
set opt(chan)           Channel/Sat
set opt(bw_up)		1Mb
set opt(bw_down)	1Mb
set opt(phy)            Phy/Sat
set opt(mac)            Mac/Sat/ZDPA
set opt(ifq)            Queue/DropTail
set opt(qlim)		50
set opt(ll)             LL/Sat
set opt(wiredRouting)   OFF

# XXX This tracing enabling must precede link and node creation 
set outfile [open out.tr w]

$ns trace-all $outfile

# Set up satellite and terrestrial nodes

# GEO satellite at 0 degrees longitude 
$ns node-config -satNodeType geo-repeater \
		-llType $opt(ll) \
		-ifqType $opt(ifq) \
		-ifqLen $opt(qlim) \
		-macType $opt(mac) \
		-phyType $opt(phy) \
		-channelType $opt(chan) \
		-downlinkBW $opt(bw_down) \
		-wiredRouting $opt(wiredRouting)
set n1 [$ns node]
$n1 set-position 0


# Place 50 terminals at 100 different locations
$ns node-config -satNodeType terminal
set num_nodes $n_node_
for {set a 1} {$a <= $num_nodes} {incr a} {
	set n($a) [$ns node]
	$n($a) set-position [expr -15 + $a * 0.1] [expr 15 - $a * 0.1]
	$n($a) add-gsl geo $opt(ll) $opt(ifq) $opt(qlim) $opt(mac) $opt(bw_up) \
  		$opt(phy) [$n1 set downlink_] [$n1 set uplink_]
}

for {set a 1} {$a <= $num_nodes} {incr a} {
	set b [expr int($a + (0.5 * $num_nodes))]
	if {$b > $num_nodes} {
		incr b [expr -1 * $num_nodes]
	}

	set udp($a) [new Agent/UDP]
	$ns attach-agent $n($a) $udp($a)
	set exp($a) [new Application/Traffic/Exponential]
	$exp($a) attach-agent $udp($a)
	$exp($a) set rate_ 1Kb
	if {$test_ == "poisson"} {
		$exp($a) set rate_ 10Mb
		$exp($a) set burst_time_ 0
		$exp($a) set idle_time_ $t_idle_
	}

	set null($a) [new Agent/Null]
	$ns attach-agent $n($b) $null($a)

	$ns connect $udp($a) $null($a)
	$ns at 1  "$exp($a) start"
}

$ns trace-all-satlinks $outfile

# We use centralized routing
set satrouteobject_ [new SatRouteObject]
$satrouteobject_ compute_routes

$ns at 6.5 "finish"

proc finish {} {

set f [open DRA-test/test2/record2.txt a]
set pracotl [new PracOfferedTrafficLoad] 
set pracload [$pracotl returnPracLoad]
set retranstimeotl [new RetransTimes]
set retranstime [$retranstimeotl returnRetransTimeSum]
set pracsentpkt [$retranstimeotl returnPracSentPkt]
puts $f "Practical sent bits $pracload"
puts $f "DRA RetranstimesSum $retranstime" 
puts $f "PracSentPkt $pracsentpkt"

close $f
	global ns outfile 
	$ns flush-trace
	close $outfile
	exit 0
}

$ns run

