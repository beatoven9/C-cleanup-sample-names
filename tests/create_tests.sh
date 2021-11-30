#!/bin/bash

SHARPS_OR_FLATS=$1

declare -a NOTES_WITH_SHARPS
declare -a NOTES_WITH_FLATS

PREFIX=piano_samples123

function Populate_Arrays(){
    NOTES_WITH_SHARPS=(A C D F G)
    NOTES_WITH_FLATS=(A B D E G)
}

function Clear_Previous_Files(){
    #Is there already data in these folders?
    previouFiles=$(shopt -s nullglob dotglob; echo test_input/*)
    if (( ${#previouFiles} ))
    then
        echo "Deleting old data"
        rm test_input/*
    else
        echo "No data in folder"
    fi
}

function CheckArgs(){
    if [[ $SHARPS_OR_FLATS = "sharps" ]]
    then
        echo 'Creating files with sharps.'
    elif [[ $SHARPS_OR_FLATS = "flats" ]]
    then
        echo 'Creating files with flats.'
    else
        echo "use 'sharps' or 'flats' as an argument"
        exit
    fi
}

function Create_Test_Set(){
    for octave in 0 1 2 3 4 5 6 7 8
    do
        for note in A B C D E F G
        do

            touch ./test_input/${PREFIX}_${note}-${octave}.wav
            if [[ $SHARPS_OR_FLATS = 'sharps' ]]
            then
                for altered_note in ${NOTES_WITH_SHARPS[@]}
                do
                    if [[ $altered_note = $note ]]
                    then
                        touch ./test_input/${PREFIX}_${note}'#'-${octave}.wav
                    fi
                done
            fi

            if [[ $SHARPS_OR_FLATS = 'flats' ]]
            then
                for altered_note in ${NOTES_WITH_FLATS[@]}
                do
                    if [[ $altered_note = $note ]]
                    then
                        touch ./test_input/${PREFIX}_${note}-b${octave}.wav
                    fi
                done
            fi

        done
    done

}

function List_Dir_Contents(){
    ls test_input
}

CheckArgs
Populate_Arrays
Clear_Previous_Files
Create_Test_Set
List_Dir_Contents
