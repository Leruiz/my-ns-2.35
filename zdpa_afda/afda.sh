#!/bin/bash

fil="zdpa_afda/afda_record.txt"
filawk="zdpa_afda/throughput.awk"
atcl="zdpa_afda/afda.tcl"

echo "AFDA" >> $fil
echo "num_node = 100  idle_time = 6" >> $fil
ns $atcl poisson 100 6
awk -f $filawk out.tr >> $fil

echo "num_node = 50  idle_time = 2.5" >> $fil
ns $atcl poisson 50 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 100  idle_time = 2.5" >> $fil
ns $atcl poisson 100 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 150  idle_time = 2.5" >> $fil
ns $atcl poisson 150 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 2.5" >> $fil
ns $atcl poisson 200 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 2.5" >> $fil
ns $atcl poisson 300 2.5
awk -f $filawk out.tr >> $fil

echo "num_node = 200  idle_time = 1.5" >> $fil
ns $atcl poisson 200 1.5
awk -f $filawk out.tr >> $fil

echo "num_node = 300  idle_time = 1.5" >> $fil
ns $atcl poisson 300 1.5
awk -f $filawk out.tr >> $fil


