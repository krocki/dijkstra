#!/bin/bash
ADDR=(https://algs4.cs.princeton.edu/44sp/tinyEWD.txt
      https://algs4.cs.princeton.edu/44sp/mediumEWD.txt
      https://algs4.cs.princeton.edu/44sp/1000EWD.txt
      https://algs4.cs.princeton.edu/44sp/10000EWD.txt
      https://algs4.cs.princeton.edu/44sp/largeEWD.txt)

mkdir -pv data && cd data
for f in ${ADDR[@]}; do
  wget $f
done;

cd ..
