#!/bin/bash/

# argument = commit name

if [ "$#" -eq 0 ]; then
    commit=ZoV
elif [ "$#" -eq 1 ]; then
    commit=$1
else
    echo "Too many arguments."
    echo "Expected only commit name."
    exit 0
fi

bash bash/git/copy.bash .

git pull origin main
git add --all
git commit -m "$commit"
git push --all
