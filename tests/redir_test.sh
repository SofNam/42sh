#!/bin/sh

# Redirection test
# 1
sh -c "echo bar > /tmp/ref"
./42sh -c "echo bar > /tmp/res"

if diff /tmp/ref /tmp/res;
then
    echo "REDIRECTION_GREAT: OK"
else
    echo "REDIRECTION_GREAT: KO"
    exit 1
fi
# 2
sh -c "echo foo >> /tmp/ref"
./42sh -c "echo foo >> /tmp/res"

if diff /tmp/ref /tmp/res;
then
    echo "REDIRECTION_DGREAT: OK"
else
    echo "REDIRECTION_DGREAT: KO"
    exit 1
fi
