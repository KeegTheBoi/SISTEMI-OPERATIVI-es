#!/bin/bash

for file in $(ls /usr/include/linux/*f*.h)
do
    len=$(cat $file | wc -l )
    if (( len < 100 && len > 10)) 
    then
        tail -v -n 1 $file
    fi
done