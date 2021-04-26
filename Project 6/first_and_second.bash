#!/bin/bash

for t in 0 1 2 3 4 5 6 7 8 9 10 11 12 13
do
    for s in 8 16 32 64 128 256 512
    do
        g++ -DLOCAL_SIZE=$s -DNMB=$t -o first_and_second first_and_second.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
        ./first_and_second
    done >> second.csv
    echo '===='
done >> second.csv
