#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>

#define MAXPATHLEN 255
#define MAXDIRCAPACITY 511
#define MAXFILENAME 127

char* getFilesFromDir(char *dirToOpen, char filesList[MAXDIRCAPACITY][MAXFILENAME]);
char* newFileNames(char oldFilesList[MAXDIRCAPACITY][MAXFILENAME], char newFilesList[MAXDIRCAPACITY][MAXFILENAME]);
void printStringsInArray(char myArray[MAXDIRCAPACITY][MAXFILENAME]);

int main(int argc, char *argv[]) {
    if ( argc == 2){
        printf("The argument supplied is %s\n", argv[1]);
    } else if( argc > 2 ){
        fprintf(stderr, "Too many arguments supplied!\n");
    } else {
        fprintf(stderr, "One Argument Expected! sharps | flats\n");
        exit(1);
    }

    char filesList[MAXDIRCAPACITY][MAXFILENAME];
    char newFilesList[MAXDIRCAPACITY][MAXFILENAME];

    if(getFilesFromDir(argv[1], filesList)){
        printf("Directory opened successfully\n");
    } else {
        fprintf(stderr, "Issue opening up directory!\n");
        return 1;
    }

    if(newFileNames(filesList, newFilesList) != NULL){
        printf("Successfully made new names for files!\n");
        printStringsInArray(newFilesList);

    } else {
        fprintf(stderr, "Unable to make new filenames!\n");
    }

    //printStringsInArray(newFilesList);
}

char* getFilesFromDir(char *dirToOpen, char filesList[MAXDIRCAPACITY][MAXFILENAME]){
    char dirPath[MAXPATHLEN] = "./";
    strcat(dirPath, dirToOpen);
    printf("%s\n", dirPath);

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
        strcpy(filesList[fileListHead], dp->d_name);
        fileListHead++;
    }
    strcpy(filesList[fileListHead], "\0");
    closedir(directory);

    return *filesList;
}

char* newFileNames(char oldFilesList[MAXDIRCAPACITY][MAXFILENAME], char newFilesList[MAXDIRCAPACITY][MAXFILENAME]){
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
    while(strcmp(oldFilesList[x], "\0") != 0){
        char *oldFile = oldFilesList[x];

        strtok(oldFile, ".");
        char *dotDelimiter = ".";
        char *fileNameExt = strtok(NULL, dotDelimiter);

        char *newFileName = (char*) malloc(MAXFILENAME);

        errorFlag = regexec(&regObject, oldFile, nmatch, matchInfo, REG_NOTEOL);
        if (!errorFlag) {
            printf("match found! \n");

            strncpy(newFileName, &oldFilesList[x][matchInfo[0].rm_so], MAXFILENAME);

            int endOfString = matchInfo[0].rm_eo - matchInfo[0].rm_so;
            newFileName[endOfString] = '\0';

            //Use strtok to eliminate - and _ from the note names
            char *delimiters = "-_";
            char *tokenA = strtok(newFileName, delimiters);
            char *tokenB;
            if ((tokenB = strtok(NULL, delimiters))){
                strncpy(newFileName, tokenA, MAXFILENAME);
                strncat(newFileName, tokenB, MAXFILENAME);
            }
            char *dot = (char*) malloc(MAXFILENAME);
            strncpy(dot, ".", MAXFILENAME);

            strncat(dot, fileNameExt, MAXFILENAME);
            strncat(newFileName, dot, MAXFILENAME);
        } else if (errorFlag==REG_NOMATCH) {
            printf("No matches found\n");
        } else {
            printf("unknown error\n");
        }
        
        char newPrefix[MAXFILENAME] = "AwesomeSample_";
        strncat(newPrefix, newFileName, MAXFILENAME);
        strncpy(newFilesList[x], newPrefix, MAXFILENAME);
        x++;

    }
    return *newFilesList;
}

void printStringsInArray(char myArray[MAXDIRCAPACITY][MAXFILENAME]){ 
    int x = 0;
    while(strcmp(myArray[x], "\0") != 0){
        printf("%s\n", myArray[x]);
        x++;
    }
}
