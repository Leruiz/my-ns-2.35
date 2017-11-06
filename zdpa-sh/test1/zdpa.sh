#!/bin/bash


#test 1 num nodes = 100

fil="zdpa-records/record1.txt"
filawk="zdpa-sh/throughput.awk"
filtcl="zdpa-sh/test1/zdpa-tcl-group.tcl"
echo "test 1, pkt lenght = 168 bits" > $fil 

# test k 

echo "k = 4, r = 1, p = 0.75" >> $fil
ns $filtcl poisson 4 1 0.75
awk -f $filawk out.tr >> $fil

echo "k = 6, r = 1, p = 0.75" >> $fil
ns $filtcl poisson 6 1 0.75
awk -f $filawk out.tr >> $fil

echo "k = 8, r = 1, p = 0.75" >> $fil
ns $filtcl poisson 8 1 0.75
awk -f $filawk out.tr >> $fil


#test r
echo "k = 6, r = 0.25, p = 0.75" >> $fil 
ns $filtcl poisson 6 0.25 0.75
awk -f $filawk out.tr >> $fil

echo "k = 6, r = 0.5, p = 0.75" >> $fil 
ns $filtcl poisson 6 0.5 0.75
awk -f $filawk out.tr >> $fil

echo "k = 6, r = 0.75, p = 0.75" >> $fil
ns $filtcl poisson 6 0.75 0.75
awk -f $filawk out.tr >> $fil


# test p
echo "k = 6, r = 0.5, p = 0" >> $fil
ns $filtcl poisson 6 0.5 0
awk -f $filawk out.tr >> $fil

echo "k = 6, r = 0.5, p = 0.25" >> $fil
ns $filtcl poisson 6 0.5 0.25
awk -f $filawk out.tr >> $fil

echo "k = 6, r = 0.5, p = 0.5" >> $fil
ns $filtcl poisson 6 0.5 0.5
awk -f $filawk out.tr >> $fil

# echo "k = 6, r = 0.5, p = 0.75" >> $fil have tested
