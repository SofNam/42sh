#!/bin/sh

alias l=ls

l Makefile.am

alias say=echo

say Hello World!

alias peace_out=exit

unalias -f

unalias say

say Hello

alias

unalias -a

alias

alias peace_out=exit

peace_out 42
