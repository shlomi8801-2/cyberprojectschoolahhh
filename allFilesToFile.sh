#!/bin/bash
find . -print0 -type f | while read -d $'\0' file
do
    echo "file: ${file#"./"}" #remove the first 2 characters that are "./"
    cat $file # print the file contents
    echo "" # new line
done