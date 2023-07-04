#!/usr/bin/sh

for i in 1 2 3
do
    for i in 1 2
    do
        if ls Makefile
        then
            echo hi
            continue 0
        fi
    done
done

echo hi there
