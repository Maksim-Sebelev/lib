#!/bin/bash

# arguments = dirs or files for copy in dir .copys
# exit codes
# 0 - success
# 1 - no arguments
# 

if [ "$#" -eq 0 ]; then
    echo "Expected arguments."
    exit 1;
fi

copy_dir=../.copys
mkdir -p $copy_dir

if [ "$1" == "clean" ]; then
    read -p "Are you sure, that you want to REMOVE all dir for copys? [y/n]: " user_input
    if [ "$user_input" == "y" ] || [ -z "$user_input" ]; then
        rm -rf $copy_dir/{*,.[!.]*,..?*}
    fi
    exit 0;
fi

for arg in "$@"; do

    finally_copy_dir=$copy_dir
    finally_arg=$arg

    if [ -f "$arg" ]; then
        cp $finally_arg $finally_copy_dir/ &> /dev/null
    elif [ -d "$arg" ]; then
        if [ "$arg" == "." ]; then
            now_dir=$(basename "$(pwd)")
            finally_arg=$now_dir
            finally_copy_dir=$copy_dir/$finally_arg
        fi
        

        while [ -d "$finally_copy_dir/" ]; do
            finally_copy_dir=$finally_copy_dir".2"
    
            if [[ ! -d "$finally_copy_dir/$finally_arg" ]]; then
                mkdir -p $finally_copy_dir/
                break
            fi

            echo "'$finally_copy_dir/$finally_arg' already exists in .copys/."
            echo "will be made '$finally_copy_dir/$finally_arg'"
        done

        mkdir -p $finally_copy_dir
        cp -r $arg $finally_copy_dir/ &> /dev/null
    else
        echo "$arg - doesn't exists"
    fi
done
