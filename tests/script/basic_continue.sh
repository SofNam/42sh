#!/bin/sh

for i in 1 2 3 4 5
do
    if echo foo
    then
        continue
    fi
    echo hello world!
done
