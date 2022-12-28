#!/bin/bash
if [ $# -lt 1 ]
then
  echo "No filename specified."
  exit 1
fi

f=`basename -s .s $1`
if [ -f $f.s ]
then
  nasm -f elf32 $f.s -o $f.o && ld -m elf_i386 $f.o -o $f && ./$f
else
  echo "There is no $f.s file in here"
  exit 1
fi