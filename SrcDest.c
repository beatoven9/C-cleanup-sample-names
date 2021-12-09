#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <unistd.h>

#include "Constants.h"
#include "SrcDest.h"

struct srcDestFile* sd_NewFileNames(struct srcDestFile sd_list[]){
    int x = 0;
    regex_t regObject;
    int errorFlag;
    char* regex = "[a-g,A-G][#-b]?[-_]?[0-9]";
    size_t nmatch = 1;
    regmatch_t matchInfo[1];

    //This function regcomp() defaults to using Basic Regular Expressions and I need Extended with this regex
    errorFlag = regcomp(&regObject, regex, REG_EXTENDED);
    if (errorFlag){
        fprintf(stderr, "Problem compiling the regular expression!\n");
        exit(1);
    }

    // I previously marked the last element of the oldFilesList to be "EndOfArray" so that I can check for it 
    // and won't check an index past the last one here.
    while(strcmp(sd_list[x].src, "EndOfArray") != 0){
        char *oldFile = sd_list[x].src;
        //Don't check . or ..
        if((strcmp(sd_list[x].src, ".") == 0) || (strcmp(sd_list[x].src, "..")) == 0){
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
            strncpy(newFileName, &sd_list[x].src[matchInfo[0].rm_so], MAXFILENAME);

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
        strncpy(sd_list[x].dest, newPrefix, MAXFILENAME);
        x++;
    }
    x++;
    strncpy(sd_list[x].dest, "EndOfArray", MAXFILENAME);
    return sd_list;
}

void sd_PrintDestinationFiles(struct srcDestFile sd_file_list[]){
    int i = 0;
    while (strcmp(sd_file_list[i].dest, "EndOfArray") != 0){
        printf("Destination: %s\n", sd_file_list[i].dest);
        i++;
    }

}

char* NewFileNames(char oldFilesList[MAXDIRCAPACITY][MAXFILENAME], char newFilesList[MAXDIRCAPACITY][MAXFILENAME]){
    int x = 0;
    regex_t regObject;
    int errorFlag;
    char* regex = "[a-g,A-G][#-b]?[-_]?[0-9]";
    size_t nmatch = 1;
    regmatch_t matchInfo[1];

    //This function regcomp() defaults to using Basic Regular Expressions and I need Extended with this regex
    errorFlag = regcomp(&regObject, regex, REG_EXTENDED);
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

struct srcDestFile* sd_GetFilesFromDir(struct srcDestFile sd_dir, struct srcDestFile sd_file_list[MAXFILENAME]){
    //this is a bug if abs path is already obtained
    //char dirPath[MAXPATHLEN] = "./";
    //strcat(dirPath, sd_dir.src);
    //**************************

    DIR *directory = opendir(sd_dir.src);
    if (directory == NULL ){
        fprintf(stderr, "Could not open directory!\n");
        return NULL;
    }

    //This struct will be returned by readdir. contains d_name (array of filenames as arrays of chars)
    struct dirent *dp;

    int fileListHead = 0;
    while ((dp = readdir(directory)) != NULL )
    {
        strncpy(sd_file_list[fileListHead].src, dp->d_name, MAXFILENAME);
        fileListHead++;
    }
    strncpy(sd_file_list[fileListHead].src, "EndOfArray", MAXFILENAME);
    closedir(directory);

    return sd_file_list;
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

void BuiltInCopy(struct srcDestFile sd){
    char command[MAXFILENAME+1];
    strncpy(command, "cp ", MAXFILENAME);

    strncat(command, sd.src, MAXFILENAME);
    strncat(command, " ", MAXFILENAME);
    strncat(command, sd.dest, MAXFILENAME);
}

void PrintSrcDestDir(struct srcDestDir *sd){
    printf("\tSource path is: %s\n\tDestination path is: %s\n", sd->src, sd->dest);
    return;
}

void PrintSrcDestFile(struct srcDestFile *sd){
    printf("\tSource path is: %s\n\tDestination path is: %s\n", sd->src, sd->dest);
    return;

}

