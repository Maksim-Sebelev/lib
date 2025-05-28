#!/bin/bash/

mkdir -p project_struct/

tree -I 'bash|bin|build|dot|Log' > project_struct/project_struct.txt 
