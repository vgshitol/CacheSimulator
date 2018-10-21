#!/usr/bin/env bash

#Graph 1 and 2
declare -i n

for i in 0 1 2 3 4 5 6 7 8 9 10
do
  n=1024*2**${i};
for j in 1 2 4 8 n
do
  make clean
  make
  ./sim_cache 32 ${n} ${j} 0 0 0 gcc_trace.txt > output${i}.txt
  echo ${n}
  echo "Looping ... number $i"
done
done

#Graph 3
declare -i k;

for i in 0 1 2 3 4 5 6 7 8
do
  n=1024*2**${i};
  k = 1024*512;
for j in 1 2 4 8 n
do
  make clean
  make
  ./sim_cache 32 ${n} ${j} 0 l 8 gcc_trace.txt > output${i}.txt
  echo ${n}
  echo "Looping ... number $i"
done
done

#Graph 4
declare -i k;

for i in 0 1 2 3 4 5 6 7 8
do
  n=1024*2**${i};
for j in 4 5 6 7
do
  k = 2**${j};
  make clean
  make
  ./sim_cache ${k} ${n} 4 0 0 0 gcc_trace.txt > output${i}.txt
  echo ${n}
  echo "Looping ... number $i"
done
done