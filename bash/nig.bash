#!/bin/bash/

# nig = new in .gitignore
# remove file, that was push in git rep and after was add in gitignore

red_concole_color="\x1b[31m"
console_color_reset="\x1b[0m"

if [ "$#" -eq 0 ]; then
    echo -e "$red_concole_color no arguments."
    echo -e "expected files and/or directories.$console_color_reset"
    exit 0
fi

for arg in "$@"; do
    # arg="${arg%/}" # remove '/' from end of directory name

    if [ -f $arg ]; then
        echo "removing file: '$arg'"
        git rm --cached "$arg"

    elif [ -d "$arg" ]; then
        echo "removing directory: '$arg'"
        git rm --cached -r "$arg"

    else
        echo -e "$red_concole_color'$arg' doesn't exist.$console_color_reset"
    fi
done