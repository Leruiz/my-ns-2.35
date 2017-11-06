#!/bin/bash


#test 2

fil="zdpa-trunc/zdpa-records/record2.txt"
filawk="zdpa-trunc/throughput.awk"
filtcll="zdpa-trunc/test2/zdpa-tcl-group.tcl"

echo "test2 cosntruction k = 6, r = 0.5, p = 0.5" >> $fil

c=0
k=6
r=0.5
p=0.25


echo "num_node = 200  idle_time = 1.5" >> $fil
ns $filtcll poisson $c $k $r $p 200 1.5
awk -f $filawk out.tr >> $fil
