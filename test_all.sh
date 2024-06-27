#!/bin/bash

FAILED=()

cmake --build cmake-build-release

for file in ./examples/*.rn; do
    echo $'\e[32m[test_all.sh] Running file '$file$'\e[0m'
    gtimeout 5s ./cmake-build-release/radion "$file"

    if [ 0 -ne $? ]; then
        FAILED+=($file)
        echo $'\e[31m[test_all.sh] Program ran unsuccessfully\e[0m'
    else
        echo $'\e[32m[test_all.sh] Program ran successfully\e[0m'
    fi

    echo ""
done

if [ 0 -ne ${#FAILED[@]} ]; then
    echo $'\e[31m[test_all.sh] Error running files:'
    for file in ${FAILED[@]} ; do
      echo "[test_all.sh] > $file"
    done
    echo $'\e[0m';
    exit 1
else
    echo $'\e[32m[test_all.sh] All files ran successfully\e[0m'
    exit 0
fi
