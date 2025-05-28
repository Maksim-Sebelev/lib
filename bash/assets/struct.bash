#!/bin/bash/

if [ "$#" -eq 0 ]; then
    mkdir -p project_struct/
    tree --gitignore > project_struct/project_struct.txt 
else
    echo "No arguments expected"
fi
