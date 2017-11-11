#!/bin/bash
#test 6

fil="DRA-test/test6/record6.txt"
filawk="DRA-test/throughput.awk"
filtcll="DRA-test/test6/dra-tcl-group.tcl"

echo "the C++ code of setting random interval has been modified" >> $fil
echo "test6 random packet length， windowt = 40000  (doubled)" >> $fil
echo "-----------------k = 2-----------------" >> $fil
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


echo "-----------------k = 3-----------------" >> $fil

k=3

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


echo "-----------------k = 4-----------------" >> $fil

k=4

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


echo "-----------------k = 5-----------------" >> $fil

k=5

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

echo "-----------------k = 6-----------------" >> $fil
k=6

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




