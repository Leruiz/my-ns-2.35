#!/bin/bash


#test 11

fil="zdpa-records/record11.txt"
filawk="zdpa-sh/test11/throughput.awk"
filtcll="zdpa-sh/test11/zdpa-tcl-group.tcl"

echo "num_node = 150  idle_time = 2.5" >> $fil
ns $filtcll poisson 6 0.5 0.5 150 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 100  idle_time = 2.5" >> $fil
ns $filtcll poisson 6 0.5 0.5 100 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 50  idle_time = 2.5" >> $fil
ns $filtcll poisson 6 0.5 0.5 50 2.5
awk -f $filawk out.tr >> $fil

echo "test11 k = 6, r = 0.5, p = 0.5" > $fil

echo "num_node = 200  idle_time = 2.5" >> $fil
ns $filtcll poisson 6 0.5 0.5 200 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 2.5" >> $fil
ns $filtcll poisson 6 0.5 0.5 300 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 1.5" >> $fil
ns $filtcll poisson 6 0.5 0.5 200 1.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 1.5" >> $fil
ns $filtcll poisson 6 0.5 0.5 300 1.5
awk -f $filawk out.tr >> $fil

