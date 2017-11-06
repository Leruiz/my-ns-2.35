#!/bin/bash


#test 2

fil="zdpa-trunc/zdpa-records/record2.txt"
filawk="zdpa-trunc/throughput.awk"
filtcll="zdpa-trunc/test2/zdpa-tcl-group.tcl"

echo "test2 cosntruction k = 6, r = 0.5, p = 0.5" >> $fil

c=0
k=6
r=0.5
p=0.5

echo "num_node = 100  idle_time = 6" >> $fil
ns $filtcll poisson $c $k $r $p 100 6
awk -f $filawk out.tr >> $fil

echo "num_node = 50  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 50 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 100  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 100 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 150  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 150 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 200 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 300 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 1.5" >> $fil
ns $filtcll poisson $c $k $r $p 200 1.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 1.5" >> $fil
ns $filtcll poisson $c $k $r $p 300 1.5
awk -f $filawk out.tr >> $fil

