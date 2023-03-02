#!/bin/bash

gcc code/arraylist.c tests/test_arraylist.c
valgrind ./a.out
rm ./a.out