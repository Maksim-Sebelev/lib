#!/bin/bash/

# nig = new in .gitignore
# remove files and dirs, that was push in git rep and after was add in gitignore
# args = removing dirs and files

red_concole_color="\x1b[31m"
green_console_color="\x1b[32m"
yellow_console_color="\e[0;33m"
console_color_reset="\x1b[0m"

if [ ! "$#" -eq 0 ]; then
    echo -e $red_concole_color"No arguments expected."
    exit 1
fi

remove()
{
    if [ "$#" != "2" ]; then
        echo ${red_concole_color}"bad 'remove()' args: '$@'"${console_color_reset}
        exit 2
    fi

    arg="$1"
    r_flag=""
    msg=""

    if [ "$2" == "-f" ]; then
        r_flag=""
        msg="file"

    elif [ "$2" == "-d" ]; then
        r_flag="-r"
        msg="direcory"

    else
        echo ${red_concole_color}"bad 'remove()' args: '$@'"${console_color_reset}
        exit 1

    fi

    if git ls-files --cached --error-unmatch "$arg" >/dev/null 2>&1; then
        printf "${green_console_color}removing ${msg}:\n%-30s %s\n${console_color_reset}" "${arg}" 
        git rm --cached ${r_flag} "$arg"

    else
        printf "${green_console_color}'${arg}'\n%-10s - already removed from git\n\n.${console_color_reset}"
    fi
}


git ls-files --others --ignored --exclude-standard | while read -r arg; do

    if [ -f "${arg}" ]; then
        remove "${arg}" "-f"

    elif [ -d "${arg}" ]; then
        remove "${arg}" "-d"

    else
        echo -e "${red_concole_color}'${arg}' doesn't exist.${console_color_reset}"
    fi

done