#!/bin/bash


#test 10

fil="zdpa-records/record10.txt"
filawk="zdpa-sh/test10/throughput.awk"
filtcll="zdpa-sh/test10/zdpa-tcl-group.tcl"

echo "test10 k = 6, r = 0.25, p = 0.5" > $fil

k=6
r=0.25
p=0.5

echo "num_node = 100  idle_time = 6" >> $fil
ns $filtcll poisson $k $r $p 100 6
awk -f $filawk out.tr >> $fil

echo "num_node = 50  idle_time = 2.5" >> $fil
ns $filtcll poisson $k $r $p 50 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 100  idle_time = 2.5" >> $fil
ns $filtcll poisson $k $r $p 100 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 150  idle_time = 2.5" >> $fil
ns $filtcll poisson $k $r $p 150 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 2.5" >> $fil
ns $filtcll poisson $k $r $p 200 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 2.5" >> $fil
ns $filtcll poisson $k $r $p 300 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 1.5" >> $fil
ns $filtcll poisson $k $r $p 200 1.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 1.5" >> $fil
ns $filtcll poisson $k $r $p 300 1.5
awk -f $filawk out.tr >> $fil

