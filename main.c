//to do:
//      handle commandline arguments more appropriately
//      add / to end of dir arguments if none exist
//      options

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Constants.h"
#include "SrcDest.h"

void PrintStringsInArray(char myArray[MAXDIRCAPACITY][MAXFILENAME]);

int main(int argc, char *argv[]) {
    struct srcDestFile sd_dir;
    if ( argc == 3){
        strncpy(sd_dir.src, argv[1], MAXPATHLEN);
        strncpy(sd_dir.dest, argv[2], MAXPATHLEN);

        GetAbsPaths(&sd_dir);
        PrintSrcDestFile(&sd_dir);

    } else if( argc > 3 ){
        fprintf(stderr, "Too many arguments supplied!\n");
    } else {
        fprintf(stderr, "Two Arguments Expected!\n      ./a.out sourceFolder targetFolder\n");
        exit(1);
    }

    struct srcDestFile sd_file_list[MAXDIRCAPACITY];

    if(!sd_GetFilesFromDir(sd_dir, sd_file_list)){
        fprintf(stderr, "Issue opening up directory!\n");
        return 1;
    }
    printf("Dir opened successfully!\n");

    //int x = 0;
    //while (strcmp(sd_file_list[x].src, "EndOfArray") != 0){
    //    printf("source: %s\n", sd_file_list[x].src);
    //    x++;
    //}

    if(sd_NewFileNames(sd_file_list) != NULL){
        printf("New names created\n");
    }

    PrependAbsPath(sd_dir, sd_file_list);

    sd_CopyFiles(sd_file_list);
}

void PrintStringsInArray(char myArray[MAXDIRCAPACITY][MAXFILENAME]){ 
    int x = 0;
    while(strcmp(myArray[x], "EndOfArray") != 0){
        printf("%s\n", myArray[x]);
        x++;
    }
}

