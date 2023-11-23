#!/bin/bash
#******** DO NOT EDIT THIS FILE ********#
# This shell script is for the removal of shared memory objects that 
# may not have been removed correctly 
# Usage:
#   ./runrmsho.sh
# to remove all possible project shared memory objects
# Or
#   ./runrmsho.sh -m
# to remove shared memory objects only (not semaphores)
# Or
#   ./runrmsho.sh -s
# to remove semaphore shared memory objects only
RM=rmsho

function rmshm {
    for i in none ipc_jobq mux_lockvar mux_peters test_ipc
    do
        ./$RM $i
    done
}

function rmsem {
    for i in sjq.mutex sjq.full sjq.empty
    do
        ./$RM -s $i
    done
}


if [ $# != 1 ]
then
    rmshm
    rmsem
elif [ $1 == "-s" ]
then
    rmsem
elif [ $1 == "-m" ]
then
    rmshm
else 
    rmshm
    rmsem
fi


