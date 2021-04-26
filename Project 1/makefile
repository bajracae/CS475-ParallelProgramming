#!/bin/bash

# number of threads:
for t in 1 2 3 4 5 6 7 8
do
	NUMT=$t
	# number of subdivisions:
	for s in 1 10 100 1000 10000 100000 500000
	do
		printf "\n"
		NUMTRIALS=$s
		g++ -DNUMTRIALS=$s -DNUMT=$t project1.cpp helperfunctions.cpp -o prog -lm -fopenmp
		./prog
	done
done
