#!/bin/bash

# number of threads:
for t in 1 2 4 8
do
	NUMT=$t
	for s in 32 64 128 256 512 1024 2048 4096 
	do
		NUMNODES=$s
		g++ -DNUMNODES=$s -DNUMT=$t project2.cpp -o prog -lm -fopenmp
		./prog
	done
done
