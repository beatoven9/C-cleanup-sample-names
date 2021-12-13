#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Constants.h"
#include "SrcDest.h"
#include <getopt.h>

void PrintStringsInArray(char myArray[MAXDIRCAPACITY][MAXFILENAME]);

static struct option const long_opts[] = 
{
    {"input-dir", required_argument, NULL, 'i'},
    {"output-dir", required_argument, NULL, 'o'},
    {NULL, 0, NULL, 0 }

};

char programName[] = "cleanUpFiles";

void usage (int status)
{
    printf("\
    Usage: %s --input-dir=myInputDir --output-dir=myOutputDir\n", programName);

    exit(status);
}

int main(int argc, char *argv[]) 
{
    int c;
    char input_dir[MAXPATHLEN];
    char output_dir[MAXPATHLEN];

    while ((c = getopt_long(argc, argv, "i:o:", long_opts, NULL)) != -1)
    {
        switch (c)
        {
            case 'i':
                strncpy(input_dir, optarg, MAXPATHLEN);
                break;
            case 'o':
                strncpy(output_dir, optarg, MAXPATHLEN);
                break;
            default:
                usage(EXIT_FAILURE);
        }
    }

    struct srcDestFile sd_dir;

    strncpy(sd_dir.src, input_dir, MAXPATHLEN);
    strncpy(sd_dir.dest, output_dir, MAXPATHLEN);

    GetAbsPaths(&sd_dir);
    PrintSrcDestFile(&sd_dir);

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

