#!/bin/bash
TESTS=(./data/tinyEWD)
METHODS=(./dijkstra_adj ./dijkstra_csr)

for t in ${TESTS[@]}; do
  for x in ${METHODS[@]}; do
    $x $t.txt $t.out
    diff $t.out $t.ref
    if [ $? == 0 ]
    then
    echo "$x : $t: RESULT OK"
    else
    echo "!!! $x : $t: FAIL"
    fi
  done
done
