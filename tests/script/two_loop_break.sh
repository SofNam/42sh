#!/bin/sh

for i in 1 2 3
do
    for i in 1 2 3 4 5
    do
        if ls Makefile
        then
            break 2
        fi
        echo hi there
    done
done
