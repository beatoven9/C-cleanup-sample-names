#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Constants.h"
#include "SrcDest.h"

void PrintStringsInArray(char myArray[MAXDIRCAPACITY][MAXFILENAME]);
void PrintSrcDestDir(struct srcDestDir sd);
void PrintSrcDestFile(struct srcDestFile sd);

int main(int argc, char *argv[]) {
    if ( argc == 3){
        printf("\tSource path is: %s\n\tDestination path is: %s\n", argv[1], argv[2]);

       // struct srcDestFile sd;
       // strncpy(sd.src, argv[1], MAXPATHLEN);
       // strncpy(sd.dest, argv[2], MAXPATHLEN);

       // GetAbsPaths(&sd);
       // PrintSrcDestFile(sd);

    } else if( argc > 3 ){
        fprintf(stderr, "Too many arguments supplied!\n");
    } else {
        fprintf(stderr, "Two Arguments Expected!\n      ./a.out sourceFolder targetFolder\n");
        exit(1);
    }

    char filesList[MAXDIRCAPACITY][MAXFILENAME];
    char newFilesList[MAXDIRCAPACITY][MAXFILENAME];

    if(!GetFilesFromDir(argv[1], filesList)){
        fprintf(stderr, "Issue opening up directory!\n");
        return 1;
    }

    if(NewFileNames(filesList, newFilesList) != NULL){
        //PrintStringsInArray(newFilesList);
    } else {
        fprintf(stderr, "Unable to make new filenames!\n");
    }

    PrintStringsInArray(newFilesList);
}

void PrintSrcDestFile(struct srcDestFile sd){
    printf("\tSource path is: %s\n\tDestination path is: %s\n", sd.src, sd.dest);
    return;

}

void PrintSrcDestDir(struct srcDestDir sd){
    printf("\tSource path is: %s\n\tDestination path is: %s\n", sd.src, sd.dest);
    return;
}


void PrintStringsInArray(char myArray[MAXDIRCAPACITY][MAXFILENAME]){ 
    int x = 0;
    //for(int i = 0; i < 120; i++){
    //    printf("%i\t%s\n", i, myArray[i]);
    //}
    //while(strcmp(myArray[x], "EndOfArray") != 0){
    //    printf("%i\t%s\n", x, myArray[x]);
    //    x++;
    //}
    while(strcmp(myArray[x], "EndOfArray") != 0){
        printf("%s\n", myArray[x]);
        x++;
    }
}

