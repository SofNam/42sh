#!/bin/sh

if echo yass
then
    ls Makefile
elif ls -y
then echo no
else
    echo yes
fi

# echo don't echo me, please
