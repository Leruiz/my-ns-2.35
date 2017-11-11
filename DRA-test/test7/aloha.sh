#!/bin/bash
#test 7

fil="DRA-test/test7/record7.txt"
filawk="DRA-test/throughput.awk"
filtcll="DRA-test/test7/dra-tcl-group.tcl"

echo "the C++ code of setting random interval has been modified" >> $fil
echo "test7 aloha protocol" >> $fil
k=2

echo "num_node = 100  idle_time = 6" >> $fil
ns $filtcll poisson  $k  100 6
awk -f $filawk out.tr >> $fil

echo "num_node = 50  idle_time = 2.5" >> $fil
ns $filtcll poisson  $k  50 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 100  idle_time = 2.5" >> $fil
ns $filtcll poisson  $k  100 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 150  idle_time = 2.5" >> $fil
ns $filtcll poisson  $k  150 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 2.5" >> $fil
ns $filtcll poisson  $k  200 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 2.5" >> $fil
ns $filtcll poisson  $k  300 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 1.5" >> $fil
ns $filtcll poisson  $k  200 1.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 1.5" >> $fil
ns $filtcll poisson  $k  300 1.5
awk -f $filawk out.tr >> $fil



