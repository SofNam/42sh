#!/bin/sh

$(echo echo)

cd $(pwd)
echo $(echo hi there)
echo "$(echo hi there world)"
echo '$(echo hi there)'
echo '$(ls Makefile)'

$(echo exit 42)
