#!/bin/bash/

if [ "$#" -eq 0 ]; then
    commit="ZoV"
else
    commit=$1
fi
git pull origin main
git add --all
git commit -m $commit
git push --all