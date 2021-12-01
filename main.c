#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>

#define MAXPATHLEN 255
#define MAXDIRCAPACITY 500

char* getFilesFromDir(char *dirToOpen, char filesList[MAXDIRCAPACITY][MAXNAMLEN]);
char* newFileNames(char oldFilesList[MAXDIRCAPACITY][MAXNAMLEN], char newFilesList[MAXDIRCAPACITY][MAXNAMLEN]);
void printStringsInArray(char myArray[MAXDIRCAPACITY][MAXNAMLEN]);

int main(int argc, char *argv[]) {
    if ( argc == 2){
        printf("The argument supplied is %s\n", argv[1]);
    } else if( argc > 2 ){
        printf("Too many args supplied\n");
    } else {
        printf("One argument expected\n");
    }

    char filesList[MAXDIRCAPACITY][MAXNAMLEN];
    char newFilesList[MAXDIRCAPACITY][MAXNAMLEN];

    if(getFilesFromDir(argv[1], filesList)){
        printf("Directory opened successfully\n");
    } else {
        printf("Issue opening up directory\n");
        return 1;
    }

    if(newFileNames(filesList, newFilesList) != NULL){
        printf("Successfully made new names for files!\n");
        printStringsInArray(newFilesList);

    } else {
        printf("Unable to make new file names\n");
    }

    //printStringsInArray(newFilesList);
}

char* getFilesFromDir(char *dirToOpen, char filesList[MAXDIRCAPACITY][MAXNAMLEN]){
    char dirPath[MAXPATHLEN] = "./";
    strcat(dirPath, dirToOpen);
    printf("%s\n", dirPath);

    DIR *directory = opendir(dirPath);
    if (directory == NULL ){
        printf("Could not open directory");
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

char* newFileNames(char oldFilesList[MAXDIRCAPACITY][MAXNAMLEN], char newFilesList[MAXDIRCAPACITY][MAXNAMLEN]){
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
        printf("This: %s\n", fileNameExt);

        char newFileName[MAXNAMLEN];

        errorFlag = regexec(&regObject, oldFile, nmatch, matchInfo, REG_NOTEOL);
        if (!errorFlag) {
            printf("match found! \n");

            strcpy(newFileName, &oldFilesList[x][matchInfo[0].rm_so]);

            int endOfString = matchInfo[0].rm_eo - matchInfo[0].rm_so;
            newFileName[endOfString] = '\0';

            //Use strtok to eliminate - and _ from the note names
            char *delimiters = "-_";
            char *tokenA = strtok(newFileName, delimiters);
            char *tokenB;
            if ((tokenB = strtok(NULL, delimiters))){
                strcpy(newFileName, tokenA);
                strcat(newFileName, tokenB);
            }
            char *dot = (char*) malloc(MAXNAMLEN);
            strcpy(dot, ".");

            if (strcat(dot, fileNameExt) == NULL){
                printf("hey, Something went wrong!");
                return NULL;
            }
            strcat(newFileName, dot);
        } else if (errorFlag==REG_NOMATCH) {
            printf("No matches found\n");
        } else {
            printf("unknown error\n");
        }
        
        char newPrefix[MAXNAMLEN] = "AwesomeSample_";
        if (strcat(newPrefix, newFileName) == NULL) {
            printf("Something went wrong!\n");
            return NULL;

        }
        printf("This is the string: %s\n", newPrefix);
        strcpy(newFilesList[x], newPrefix);
        x++;

    }
    return *newFilesList;
}

void printStringsInArray(char myArray[MAXDIRCAPACITY][MAXNAMLEN]){ 
    int x = 0;
    while(strcmp(myArray[x], "\0") != 0){
        printf("%s\n", myArray[x]);
        x++;
    }
}
