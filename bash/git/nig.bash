#!/bin/bash/

# nig = new in .gitignore
# remove file, that was push in git rep and after was add in gitignore

red_concole_color="\x1b[31m"
console_color_reset="\x1b[0m"

if [ "$#" -eq 0 ]; then
    echo -e $red_concole_color"No arguments."
    echo -e "Expected files and/or directories.$console_color_reset"
    exit 0
fi

for arg in "$@"; do

    if [ -f $arg ]; then

        if git ls-files --error-unmatch --quiet -- "$arg" >/dev/null 2>&1; then
            echo "removing file: '$arg'"
            git rm --cached "$arg"

        else
            echo "'$arg' already removed from repository or doesn't marked in .gitignore."

        fi

    elif [ -d "$arg" ]; then

        if git ls-files --error-unmatch --quiet -- "$arg" >/dev/null 2>&1; then
            echo "removing file: '$arg'"
            git rm --cached -r "$arg"

        else
            echo "'$arg' already removed from repository or doesn't marked in '.gitignore'."

        fi

    else
        echo -e "$red_concole_color'$arg' doesn't exist.$console_color_reset"
    fi

done