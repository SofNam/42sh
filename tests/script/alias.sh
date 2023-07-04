#!/bin/sh

zoo=bar

alias foo=echo
alias quit=exit
alias list=ls

alias
foo $zoo
alias -p

foo hi
list Makefile.am
quit 42
