#!/usr/bin/bash

#This script will take 9 potential moves and mix them up to play TicTacToe randomly. 
#The string is passed to the program which will interpret it as the users input.
#Invalid input is checked by program so there is no need to keep track of potentially bad input.
mix() {

    local a=$1 i
    mixed=
    while((${#a})); do
        ((i=RANDOM%${#a}))
        mixed+=${a:i:1}
        a=${a::i}${a:i+1}
    done
}

mix "123456789"
./bin/program user <<< "$mixed"


