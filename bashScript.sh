#!/usr/bin/env bash

declare -i n
declare -i k
declare -i assoc
mr;
cs;

make clean
make

#Graph 1

for i in 0 1 2 3 4 5 6 7 8 9 10
do
  n=1024*2**${i};
  assoc=32*2**${i}
for j in 1 2 4 8 ${assoc}
do
  ./sim_cache 32 ${n} ${j} 0 0 0 gcc_trace.txt > graph1${i}${j}.txt
    variable=$(grep 'h.' graph1${i}${j}.txt | awk '{print $6}');
    variable2=$(($i+10));
    variable3=$(($j));
    echo "$variable3,$variable2,$variable" >> graph1.csv
  echo "Looping ... number $n $i $j"
done
done

## graph 2
#for i in 0 1 2 3 4 5 6 7 8 9 10
#do
#  n=1024*2**${i};
#  assoc=32*2**${i}
#for j in 1 2 4 8 ${assoc}
#do
#  ./sim_cache 32 ${n} ${j} 0 0 0 gcc_trace.txt > graph2${i}${j}.txt
#    variable=$(grep 'h.' graph2${i}${j}.txt | awk '{print $6}');
#    variable2=$(($i+10));
#    echo "$variable2,$variable" >> graph2.csv
#  echo "Looping ... number $n $i $j"
#done
#done
#
#
#Graph 3
#
#k=1024*512;
#for i in 0 1 2 3 4 5 6 7 8
#do
#  n=1024*2**${i};
#  assoc=32*2**${i}
#for j in 1 2 4 8 ${assoc}
#do
#  ./sim_cache 32 ${n} ${j} 0 ${k} 8 gcc_trace.txt > graph3${i}${j}.txt
#  variable=$(grep 'h.' graph3${i}${j}.txt | awk '{print $6}');
#  variable2=$(grep 'n. L2 miss rate:' graph3${i}${j}.txt | awk '{print $5}');
#  variable3=$(($i+10));
#    echo "$variable3,$variable2,$variable" >> graph3.csv
#  echo "Looping ... number $n $i $j"
#done
#done
#
##Graph 4
#for i in 0 1 2 3 4 5
#do
#  n=1024*2**${i};
#for j in 4 5 6 7
#do
#  k=2**${j};
#  ./sim_cache ${k} ${n} 4 0 0 0 gcc_trace.txt > graph4${i}${j}.txt
#    variable=$(grep 'h.' graph4${i}${j}.txt | awk '{print $6}');
#    variable2=$(($i+10));
#    variable3=$(($j));
#    echo "$variable2,$variable3,$variable" >> graph4.csv
#  echo "Looping ... number $n $i $j"
#done
#done
#
##Graph 5
#for i in 0 1 2 3 4 5 6 7 8
#do
#  n=1024*2**${i};
#for j in 5 6 7 8 9 10
#do
#  k=1024*2**${j};
#  if (($i < $j));
#  then
#    ./sim_cache 32 ${n} 4 0 ${k} 8 gcc_trace.txt > graph5${i}${j}.txt
#    variable=$(grep 'h.' graph5${i}${j}.txt | awk '{print $6}');
#    variable2=$(grep 'n. L2 miss rate:' graph5${i}${j}.txt | awk '{print $5}');
#    variable3=$(($i+10));
#    echo "$variable3,$variable2,$variable" >> graph5.csv
#    echo "Looping ... number $n $i $j"
#  fi
#done
#done
#
#
#
##Graph 6
#
#k=64*1024;
#
#for i in 0 1 2 3 4 5
#do
#  n=1024*2**${i};
#
#for j in 0 2 4 8 16
#do
#     ./sim_cache 32 ${n} 1 ${j} ${k} 8 gcc_trace.txt > graph6${i}${j}.txt
#  variable=$(grep 'h.' graph6${i}${j}.txt | awk '{print $6}');
#  variable2=$(grep 'n. L2 miss rate:' graph6${i}${j}.txt | awk '{print $5}');
#  variable4=$(grep 'f. swap request rate:' graph6${i}${j}.txt | awk '{print $5}');
#  variable3=$(($i+10));
#    echo "$variable3,$variable2,$variable4,$variable" >> graph6.csv
#  echo "Looping ... number $n $i $j"
#
#done
#     ./sim_cache 32 ${n} 2 0 ${k} 8 gcc_trace.txt > graph6${i}2.txt
# variable=$(grep 'h.' graph6${i}2.txt | awk '{print $6}');
#  variable2=$(grep 'n. L2 miss rate:' graph6${i}2.txt | awk '{print $5}');
#  variable4=$(grep 'f. swap request rate:' graph6${i}2.txt | awk '{print $5}');
#  variable3=$(($i+10));
#    echo "$variable3,$variable2,$variable4,$variable" >> graph6.csv
#  echo "Looping ... number $n $i $j"
#
#     ./sim_cache 32 ${n} 4 0 ${k} 8 gcc_trace.txt > graph6${i}4.txt
#  variable=$(grep 'h.' graph6${i}4.txt | awk '{print $6}');
#  variable2=$(grep 'n. L2 miss rate:' graph6${i}4.txt | awk '{print $5}');
#  variable4=$(grep 'f. swap request rate:' graph6${i}4.txt | awk '{print $5}');
#  variable3=$(($i+10));
#    echo "$variable3,$variable2,$variable4,$variable" >> graph6.csv
#  echo "Looping ... number $n $i $j"
#
#done