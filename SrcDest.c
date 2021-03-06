#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <unistd.h>

#include "Constants.h"
#include "SrcDest.h"

struct srcDestFile* sd_NewFileNames(struct srcDestFile sd_list[], char *newFilePrefix){
    int x = 0;
    char newPrefix[MAXFILENAME];
    regex_t regObject;
    int errorFlag;
    char* regex = "[a-g,A-G][#b-]?[-_]?[0-9]";
    size_t nmatch = 1;
    regmatch_t matchInfo[1];

    strncpy(newPrefix, newFilePrefix, MAXFILENAME);

    //This function regcomp() defaults to using Basic Regular Expressions and I need Extended with this regex
    errorFlag = regcomp(&regObject, regex, REG_EXTENDED);
    if (errorFlag){
        fprintf(stderr, "Problem compiling the regular expression!\n");
        exit(1);
    }

    // I previously marked the last element of the oldFilesList to be "EndOfArray" so that I can check for it 
    // and won't check an index past the last one here.
    while(strcmp(sd_list[x].src, "EndOfArray") != 0){
        char oldFileCpy[MAXFILENAME];
        strncpy(oldFileCpy, sd_list[x].src, MAXFILENAME);

        char *dotDelimiter = ".";

        strtok(oldFileCpy, dotDelimiter);
        char *fileNameExt = strtok(NULL, dotDelimiter);

        char *newFileName = (char*) malloc(MAXFILENAME);

        errorFlag = regexec(&regObject, oldFileCpy, nmatch, matchInfo, REG_NOTEOL);
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
            fprintf(stderr, "No matches found for file name: %s\n", oldFileCpy);
        } else {
            fprintf(stderr, "unknown error\n");
        }
        
        char newName[MAXFILENAME];
        strncpy(newName, newPrefix, MAXFILENAME);
        strncat(newName, newFileName, MAXFILENAME);
        strncpy(sd_list[x].dest, newName, MAXFILENAME);
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
        if ((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0)){
            continue;
        }
        
        strncpy(sd_file_list[fileListHead].src, dp->d_name, MAXFILENAME);
        fileListHead++;
    }
    strncpy(sd_file_list[fileListHead].src, "EndOfArray", MAXFILENAME);
    closedir(directory);

    return sd_file_list;
}

void sd_CopyFiles(struct srcDestFile *filesList){
    int x = 0;
    while (strcmp(filesList[x].src, "EndOfArray")){
        BuiltInCopy(filesList[x]);
        x++;
    }
}

void BuiltInCopy(struct srcDestFile sd){
    char command[MAXFILENAME+1];
    strncpy(command, "cp ", MAXFILENAME);

    strncat(command, sd.src, MAXFILENAME);
    strncat(command, " ", MAXFILENAME);
    strncat(command, sd.dest, MAXFILENAME);

    system(command);
}

struct srcDestFile* PrependAbsPath(struct srcDestFile absPath, struct srcDestFile *filesList){
    struct srcDestFile tmp_sd_file_list[MAXDIRCAPACITY];
    int x = 0;
    while (strcmp(filesList[x].src, "EndOfArray") != 0){

        strncpy(tmp_sd_file_list[x].src, "'", MAXFILENAME);
        strncat(tmp_sd_file_list[x].src, absPath.src, MAXFILENAME);
        strncat(tmp_sd_file_list[x].src, filesList[x].src, MAXFILENAME);
        strncat(tmp_sd_file_list[x].src, "'", MAXFILENAME);

        strncpy(tmp_sd_file_list[x].dest, "'", MAXFILENAME);
        strncat(tmp_sd_file_list[x].dest, absPath.dest, MAXFILENAME);
        strncat(tmp_sd_file_list[x].dest, filesList[x].dest, MAXFILENAME);
        strncat(tmp_sd_file_list[x].dest, "'", MAXFILENAME);

        strncpy(filesList[x].src, tmp_sd_file_list[x].src, MAXFILENAME);
        strncpy(filesList[x].dest, tmp_sd_file_list[x].dest, MAXFILENAME);
        x++;
    }
    return filesList;
}

void PrintSrcDestDir(struct srcDestDir *sd){
    printf("\tSource path is: %s\n\tDestination path is: %s\n", sd->src, sd->dest);
    return;
}

void PrintSrcDestFile(struct srcDestFile *sd){
    printf("\tSource path is: %s\n\tDestination path is: %s\n", sd->src, sd->dest);
    return;

}

