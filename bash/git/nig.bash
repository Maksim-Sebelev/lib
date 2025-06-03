#!/bin/bash/

# nig = new in .gitignore
# remove files and dirs, that was push in git rep and after was add in gitignore
# args = removing dirs and files

red_concole_color="\x1b[31m"
green_console_color="\x1b[32m"
yellow_console_color="\e[0;33m"
console_color_reset="\x1b[0m"

if [ "$#" -eq 0 ]; then
    echo -e $red_concole_color"No arguments."
    echo -e "Expected files and/or directories.$console_color_reset"
    exit 0
fi

for arg in "$@"; do

    if [ -f "$arg" ]; then

        if git ls-files --cached --error-unmatch "$arg" >/dev/null 2>&1; then
            echo -e $green_console_color"removing file: '$arg'"$console_color_reset
            git rm --cached "$arg"

        else
            echo -e $green_console_color"'$arg' already removed from repository or doesn't marked in .gitignore."$console_color_reset

        fi

    elif [ -d "$arg" ]; then

        if git ls-files --cached --error-unmatch "$arg" >/dev/null 2>&1; then
            echo -e $green_console_color"removing file: '$arg'"$console_color_reset
            git rm --cached -r "$arg"

        else
            echo -e $green_console_color"'$arg' already removed from repository or doesn't marked in .gitignore."$console_color_reset

        fi

    else
        echo -e "$red_concole_color'$arg' doesn't exist.$console_color_reset"
    fi

done