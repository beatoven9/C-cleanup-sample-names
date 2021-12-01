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

    if(newFileNames(filesList, newFilesList)){
        printf("Successfully made new names for files!\n");
        //printStringsInArray(newFilesList);

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

    //This function defaults to using Basic Regular Expressions and I need Extended with this regex
    errorFlag = regcomp(&regObject, "[ABCDEFG][#b]?[-_][0-9]", REG_EXTENDED);
    if (errorFlag){
        fprintf(stderr, "Problem compiling the regular expression!\n");
        exit(1);
    }


    while(strcmp(oldFilesList[x], "\0") != 0){
        const char *oldFile = oldFilesList[x];

        errorFlag = regexec(&regObject, oldFile, nmatch, matchInfo, REG_NOTEOL);
        if (!errorFlag) {
            printf("match found! %s\n", &oldFilesList[x][matchInfo[0].rm_so]);
            printf("At index: %i\n", matchInfo[0].rm_so);
            printf("Ending index: %i\n", matchInfo[0].rm_eo);
        } else if (errorFlag==REG_NOMATCH) {
            printf("No matches found\n");
        } else {
            printf("unknown error\n");
        }
        
        char newPrefix[MAXNAMLEN] = "AwesomeSample_";
        strcat(newPrefix, oldFile);
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
