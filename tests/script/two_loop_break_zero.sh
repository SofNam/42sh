#!/usr/bin/sh

for i in 1 2 3
do
    for i in 1 2
    do
        if ls Makefile
        then
            echo stupid
            break 0
        fi
    done
done

echo hello world
