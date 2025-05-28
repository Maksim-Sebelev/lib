#!/bin/bash/

mkdir -p assets
mkdir -p assets/project_struct

tree -I 'bash|bin|build|dot|Log' >> assets/project_struct/project_struct.txt 
