#!/bin/bash



if [ "$#" -eq 0 ]; then
    echo "Expected argument"
    exit 1
else if [ "$#" != "1" ]; then
    echo "Too many arguments"
    exit 1
fi

scp "$1" /usr/local/bin
