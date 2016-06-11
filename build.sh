#!/usr/bin/env bash

# Cleaning work area
rm a.out
rm project.tab.c
rm project.tab.h
rm lex.yy.c

# Project building
bison -d -t --report=all project.y
flex project.l
gcc -g -Lnode.h -Lset.h -Ldata.h -Lproject.tab.c lex.yy.c


# Test cases
echo "Commencing test case 1"
./a.out tests/test1 

# Valgrind test
echo "Valgrind test"
valgrind ./a.out tests/test1

exit
