#!/bin/bash
##128##

rm -r clover\_leaf.128.*
rm -rf clover\_leaf128

mkdir -p CloverLeaf\_new\_exp/128/cnm/
mkdir -p CloverLeaf\_new\_exp/128/rcr/

srun -N 6 -n 128 ./pingpong_least_stable

python FileTranslate.py --ST ./rcr-minor-placement/RCR.CloverLeaf128.result --layout nodefile
python FileTranslate.py --ST ./rcr-r-placement/RCR\_R.CloverLeaf128.result --layout nodefile


#round_1
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/1

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/1

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/1


#round_2
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/2

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/2

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/2


#round_3
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/3

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/3

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/3



#round_4
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/4

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/4

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/4

#round_5
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/5

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/5

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/5


#round_6
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/6

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/6

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/6


#round_7
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/7

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/7

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/7


#round_8
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/8

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/8

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/8



#round_9
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/9

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/9

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/9

#round_10
srun -N 6 -n 128 ./clover\_leaf  
wait
mv clover\_leaf.128.*  CloverLeaf\_new\_exp/128/nature-new/10

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-minor/10

srun -N 6 -n 128 -m arbitrary -w ./nodefile.RCR\_R.CloverLeaf128.result ./clover\_leaf
wait
mv clover\_leaf.128.* CloverLeaf\_new\_exp/128/rcr-r/10