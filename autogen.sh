#!/bin/sh

list="INSTALL NEWS AUTHORS ChangeLog COPYING"

touch $list
autoreconf -i
