#!/bin/bash

say_hello(){
echo "Hello, $1"
}
say_hello "chenlie"
say_hello "world"


add(){
result=$(( $1 + $2 ))
echo $result
}

sum=$(add 3 5)
echo "3 + 5 = $sum"
