#!/bin/bash
cd release
make
cd ..

FAILED = false

for file in ./examples/*.rn; do
    echo $'\e[32m[test_all.sh] Running file '$file$'\e[0m'
    gtimeout 10s ./release/radion "$file"

    if [ 0 -eq $? ]; then
        FAILED=true
        echo $'\e[31m[test_all.sh] Program ran unsuccessfully\e[0m'
    fi

    echo ""
done

if $FAILED; then
    echo $'\e[31m[test_all.sh] Error running files\e[0m'
    exit 1
else
    exit 0
fi