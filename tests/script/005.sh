#!/bin/sh

if echo hi; then
    for i in 1 2 3 4; do
        echo hi | cat
    done
elif ls Makefile; then
    case "hi" in
        "hello" | "Morning")
            echo hello && ls
            ;;
        "hi")
            echo hi
            ;;
        *)
            echo greetings
            ;;
    esac
else
    until pwd; do
        echo I am here
    done
fi

