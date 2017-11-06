#!/bin/bash

filtcll="zdpa-soliton/soliton-test-length/zdpa-tcl-group.tcl"
fil="zdpa-soliton/soliton-test-length/record.txt"
r=0.5

echo "test1 Soliton k = 6, p = 0.5" >> $fil
c=1
k=6
p=0.5

echo "num_node = 100  idle_time = 6" >> $fil
ns $filtcll poisson $c $k $r $p 100 6


echo "num_node = 50  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 50 2.5


echo "num_node = 100  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 100 2.5


echo "num_node = 150  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 150 2.5


echo "num_node = 200  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 200 2.5


echo "num_node = 300  idle_time = 2.5" >> $fil
ns $filtcll poisson $c $k $r $p 300 2.5


echo "num_node = 200  idle_time = 1.5" >> $fil
ns $filtcll poisson $c $k $r $p 200 1.5


echo "num_node = 300  idle_time = 1.5" >> $fil
ns $filtcll poisson $c $k $r $p 300 1.5


