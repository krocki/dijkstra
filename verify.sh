#!/bin/bash
TESTS=(./data/tinyEWD)

for t in ${TESTS[@]}; do
  ./dijkstra $t.txt $t.out
  diff $t.out $t.ref
  if [ $? == 0 ]
  then
  echo "$t: RESULT OK"
  else
  echo "!!! $t: FAIL"
  fi
done
