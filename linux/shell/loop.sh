#!/bin/bash
for i in 1 2 3 4 5 ; do
echo "number:$i"
done

count=0
while [ $count -lt 3 ]; do
echo "count: $count"
count=$((count + 1))
done

