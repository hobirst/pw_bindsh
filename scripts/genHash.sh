#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <password>"
    exit 1
fi

password=$1
userhash=$(echo -n $1 | sha256sum | cut -d ' ' -f1 | tr -d '\n')

echo -n '"'
for (( i=0; i < ${#userhash}; i++ )); do
    n=$(($i%2))
    if [ $n -eq 0 ]; then
        echo -n '\x'
    fi
    echo -n "${userhash:$i:1}"
done
echo -n '"'

