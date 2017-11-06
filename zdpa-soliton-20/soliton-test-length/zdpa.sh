#!/bin/bash

filtcll="zdpa-soliton-20/soliton-test-length/zdpa-tcl-group.tcl"
fil="zdpa-soliton-20/soliton-test-length/record.txt"
r=0.5

echo "test1 Soliton k = 4, p = 0.25" >> $fil
c=1
k=4
p=0.25

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


#test 2
echo "test2 Soliton k = 4,  p = 0.5" >> $fil

c=1
k=4
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

#test 3
echo "test3 Soliton k = 6,  p = 0.25" >> $fil

c=1
k=6
p=0.25

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


#test4
echo "test4 Soliton k = 6,  p = 0.5" >> $fil

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

#test5
echo "test5 Soliton k = 8,  p = 0.25" >> $fil

c=1
k=8
p=0.25

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

#test6
echo "test6 Soliton k = 8,  p = 0.5" >> $fil

c=1
k=8
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


