#!/bin/bash

gcc c/arraylist/code/arraylist.c c/arraylist/tests/test_arraylist.c
valgrind ./a.out
rm ./a.out