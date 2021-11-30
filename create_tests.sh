#!/bin/bash

ARG1=$1

declare -a NOTES_WITH_SHARPS
declare -a NOTES_WITH_FLATS

NOTES_WITH_SHARPS=(A C D F G)
NOTES_WITH_FLATS=(A B D E G)

function Populate_Arrays(){
    for NOTE in A C D F G
    do
        NOTES_WITH_SHARPS[$NOTE]=$NOTE
        echo $NOTE
    done

    for NOTE in A B D E G
    do
        NOTES_WITH_FLATS[$NOTE]=1
    done

}


function Clear_Previous_Files(){
    rm ../test_input/*
}


function CheckArgs(){
    if [[ $ARG1 = "sharps" ]]
    then
        echo 'Sharps!'
    elif [[ $ARG1 = "flats" ]]
    then
        echo 'Flats!'
    else
        echo "use 'sharps' or 'flats' as an argument"
        exit
    fi
    

}

function Create_Test_Set(){
    for OCTAVE in 0 1 2 3 4 5 6 7 8
    do
        for NOTE in A B C D E F G
        do

            touch ./test_input/piano${NOTE}${OCTAVE}.wav
            if [[ $ARG1 = 'sharps' ]]
            then
                for ALTERED_NOTE in ${NOTES_WITH_SHARPS[@]}
                do
                    if [[ $ALTERED_NOTE = $NOTE ]]
                    then
                        touch ./test_input/piano${NOTE}\#${OCTAVE}.wav
                    fi
                done
            fi

            if [[ $ARG1 = 'flats' ]]
            then
                for ALTERED_NOTE in ${NOTES_WITH_FLATS[@]}
                do
                    if [[ $ALTERED_NOTE = $NOTE ]]
                    then
                        touch ./test_input/piano${NOTE}b${OCTAVE}.wav
                    fi
                done
            fi

        done
    done

}

Clear_Previous_Files
CheckArgs
Create_Test_Set
