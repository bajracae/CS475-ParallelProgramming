#!/bin/bash

for t in 0 1 2 3 4 5 6 7 8 9 10 11 12 13
do
    for s in 32 64 128 256
    do
        g++ -DLOCAL_SIZE=$s -DNMB=$t -o third third.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
        ./third
    done >> third.csv
    echo '===='
done >> third.csv
