#!/bin/bash
if [ $# -lt 1 ]
then
  echo "No filename specified."
  exit 1
fi

f=`basename -s .s $1`
if [ -f $f.s ]
then
  nasm -f elf64 $f.s -o $f.o && ld $f.o -o $f && ./$f
else
  echo "There is no $f.s file in here"
  exit 1
fi