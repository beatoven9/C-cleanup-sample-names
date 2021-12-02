// You need to separate this into multiple files
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>
#include <unistd.h>

#define MAXPATHLEN 255
#define MAXDIRCAPACITY 255
#define MAXFILENAME 255

struct srcDestDir {
    char src[MAXPATHLEN];
    char dest[MAXPATHLEN];
};

struct srcDestFile {
    char src[MAXPATHLEN];
    char dest[MAXPATHLEN];
};

char* GetFilesFromDir(char *dirToOpen, char filesList[MAXDIRCAPACITY][MAXFILENAME]);
char* NewFileNames(char oldFilesList[MAXDIRCAPACITY][MAXFILENAME], char newFilesList[MAXDIRCAPACITY][MAXFILENAME]);
void PrintStringsInArray(char myArray[MAXDIRCAPACITY][MAXFILENAME]);
void CopyFileTo(char *src, char *dest);
struct srcDestFile* GetAbsPaths(struct srcDestFile *sd);

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

char* GetFilesFromDir(char *dirToOpen, char filesList[MAXDIRCAPACITY][MAXFILENAME]){
    char dirPath[MAXPATHLEN] = "./";
    strcat(dirPath, dirToOpen);

    DIR *directory = opendir(dirPath);
    if (directory == NULL ){
        fprintf(stderr, "Could not open directory!\n");
        return NULL;
    }

    //This struct will be returned by readdir contains d_name (array of filenames as arrays of chars)
    struct dirent *dp;

    int fileListHead = 0;
    while ((dp = readdir(directory)) != NULL )
    {
        strncpy(filesList[fileListHead], dp->d_name, MAXFILENAME);
        fileListHead++;
    }
    strncpy(filesList[fileListHead], "EndOfArray", MAXFILENAME);
    closedir(directory);

    return *filesList;
}

char* NewFileNames(char oldFilesList[MAXDIRCAPACITY][MAXFILENAME], char newFilesList[MAXDIRCAPACITY][MAXFILENAME]){
    int x = 0;
    regex_t regObject;
    int errorFlag;
    char* regex = "[a-g,A-G][#-b]";
    size_t nmatch = 1;
    regmatch_t matchInfo[1];

    //This function regcomp() defaults to using Basic Regular Expressions and I need Extended with this regex
    errorFlag = regcomp(&regObject, "[ABCDEFG][#b]?[-_]?[0-9]", REG_EXTENDED);
    if (errorFlag){
        fprintf(stderr, "Problem compiling the regular expression!\n");
        exit(1);
    }

    // I previously marked the last element of the oldFilesList to be "\0" so that I can check for it 
    // and won't check an index past the last one here.
    while(strcmp(oldFilesList[x], "EndOfArray") != 0){
        char *oldFile = oldFilesList[x];
        //Don't check . or ..
        if((strcmp(oldFilesList[x], ".") == 0) || (strcmp(oldFilesList[x], "..")) == 0){
            x++;
            printf("skipping one\n");
            continue;
        }

        strtok(oldFile, ".");
        char *dotDelimiter = ".";
        char *fileNameExt = strtok(NULL, dotDelimiter);

        char *newFileName = (char*) malloc(MAXFILENAME);

        errorFlag = regexec(&regObject, oldFile, nmatch, matchInfo, REG_NOTEOL);
        if (!errorFlag) {
            strncpy(newFileName, &oldFilesList[x][matchInfo[0].rm_so], MAXFILENAME);

            int endOfString = matchInfo[0].rm_eo - matchInfo[0].rm_so;
            newFileName[endOfString] = '\0';

            //Use strtok to eliminate - and _ from the note names
            char *delimiters = "-_";
            char *tokenA = strtok(newFileName, delimiters);
            char *tokenB;
            if ((tokenB = strtok(NULL, delimiters)) != NULL){
                //printf("Found another token\n");
                memcpy(newFileName, tokenA, MAXFILENAME);
                strncat(newFileName, tokenB, MAXFILENAME);
            }

            char dot[MAXFILENAME] = ".";
            
            strncat(dot, fileNameExt, MAXFILENAME);
            strncat(newFileName, dot, MAXFILENAME);
        } else if (errorFlag==REG_NOMATCH) {
            fprintf(stderr, "No matches found for file name: %s\n", oldFile);
        } else {
            fprintf(stderr, "unknown error\n");
        }
        
        char newPrefix[MAXFILENAME] = "AwesomeSample_";
        strncat(newPrefix, newFileName, MAXFILENAME);
        strncpy(newFilesList[x], newPrefix, MAXFILENAME);
        x++;
    }
    x++;
    strncpy(newFilesList[x], "EndOfArray", MAXFILENAME);
    return *newFilesList;
}


struct srcDestFile* GetAbsPaths(struct srcDestFile *sd){
    char path[MAXPATHLEN];
    char tmpSrc[MAXPATHLEN] = "/"; 
    char tmpDest[MAXPATHLEN] = "/";

    strncat(tmpSrc, sd->src, MAXPATHLEN);
    strncat(tmpDest, sd->dest, MAXPATHLEN);

    getcwd(path, MAXPATHLEN);

    memcpy(sd->src, path, MAXPATHLEN);
    memcpy(sd->dest, path, MAXPATHLEN);

    strncat(sd->src, tmpSrc, MAXPATHLEN);
    strncat(sd->dest, tmpDest, MAXPATHLEN);
    
    return sd;
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
        printf("%i\t%s\n", x, myArray[x]);
        x++;
    }
}

void CopyFileTo(char *src, char *dest){
    FILE *source, *destination;
    char ch;

    source = fopen(src, "r");
    if (source == NULL){
        fprintf(stderr, "Error opening source file for reading\n");
        exit(EXIT_FAILURE);
    }

    destination = fopen(dest, "w");
    if (destination == NULL){
        fclose(source);
        fprintf(stderr, "Error opening destination file for writing\n");
        exit(EXIT_FAILURE);
    }
    
    while (( ch = fgetc(source)) != EOF){
        fputc(ch, destination);
    }

    printf("Successfully copied %s to %s\n", src, dest);
    fclose(source);
    fclose(destination);

    return;
}
