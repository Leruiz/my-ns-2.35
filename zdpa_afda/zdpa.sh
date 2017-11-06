#!/bin/bash

fil="zdpa_afda/record.txt"
filawk="zdpa_afda/throughput.awk"
ztcl="zdpa_afda/zdpa-tcl-group.tcl"
atcl="zdpa_afda/afda.tcl"


echo "Non-RALR" >> $fil
c=0
k=4
r=1
p=1

echo "num_node = 100  idle_time = 6" >> $fil
ns $ztcl poisson $c $k $r $p 100 6
awk -f $filawk out.tr >> $fil

echo "num_node = 50  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 50 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 100  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 100 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 150  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 150 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 200 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 300 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 1.5" >> $fil
ns $ztcl poisson $c $k $r $p 200 1.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 1.5" >> $fil
ns $ztcl poisson $c $k $r $p 300 1.5
awk -f $filawk out.tr >> $fil


echo "Regular RALR k = 4, p = 0.5, r = 0.5" >> $fil
c=0
k=4
p=0.5
r=0.5

echo "num_node = 100  idle_time = 6" >> $fil
ns $ztcl poisson $c $k $r $p 100 6
awk -f $filawk out.tr >> $fil

echo "num_node = 50  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 50 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 100  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 100 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 150  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 150 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 200 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 300 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 1.5" >> $fil
ns $ztcl poisson $c $k $r $p 200 1.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 1.5" >> $fil
ns $ztcl poisson $c $k $r $p 300 1.5
awk -f $filawk out.tr >> $fil


echo "Random RALR k=4, p=0.5" >> $fil
c=1
k=4
p=0.5
r=0.5
echo "num_node = 100  idle_time = 6" >> $fil
ns $ztcl poisson $c $k $r $p 100 6
awk -f $filawk out.tr >> $fil

echo "num_node = 50  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 50 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 100  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 100 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 150  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 150 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 200 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 2.5" >> $fil
ns $ztcl poisson $c $k $r $p 300 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 1.5" >> $fil
ns $ztcl poisson $c $k $r $p 200 1.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 1.5" >> $fil
ns $ztcl poisson $c $k $r $p 300 1.5
awk -f $filawk out.tr >> $fil


