# 42sh: shell 42

# Description

**42sh** is a command language interpreter that shall execute commands read from
a command line string, the standard input, or a specified file.

# Authors
**Thien Phuoc Le Ngoc**
thien-phuoc.le-ngoc@epita.fr

**Yassin Bouhassoun**
yassin.bouhassoun@epita.fr

**Luu Hoang Long Vo**
luu-hoang-long.vo@epita.fr

**Thibaud Deleon**
thibaud.deleon@epita.fr

# How to
## Compile
```shell
./autogen && ./configure && make
```
Produces the `42sh` executable at the root of the directory.

## Run
```shell
./42sh [options] [command_string | file]
```

## Run test suite
```shell
make check
```

## Produce doxygen documentation
```shell
make doc
```
The index page is located at `doc/html/index.html`.

## Read the man page
```shell
man -l doc/42sh.1
```
