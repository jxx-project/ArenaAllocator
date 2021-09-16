#!/bin/sh
while true
do
    if [ $(jobs | wc -l) -lt 5 ]
    then
	utils/allocatorLoadTest --write --allocations=10000 --invocations=10000 --maxsize=255 &
    else
	sleep 1
    fi
done
