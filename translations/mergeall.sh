#!/bin/sh
list=$(ls *.po)
./prepare.sh
for file in $list; do 
  echo $file
  ./merge.sh $file
done
./build.sh