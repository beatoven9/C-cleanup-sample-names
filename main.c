#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "Constants.h"
#include "SrcDest.h"

static struct option const long_opts[] = 
{
    {"input-dir", required_argument, NULL, 'i'},
    {"output-dir", required_argument, NULL, 'o'},
    {"prefix", required_argument, NULL, 'p'},
    {NULL, 0, NULL, 0 }

};

char programName[] = "cleanUpFiles";

void usage (int status)
{
    printf("\
    Usage: %s --input-dir=myInputDir --output-dir=myOutputDir --prefix=myNewPrefix\n", programName);

    exit(status);
}

int main(int argc, char *argv[]) 
{
    int c;
    char input_dir[MAXPATHLEN];
    char output_dir[MAXPATHLEN];
    char prefix[MAXFILENAME];
    struct srcDestFile sd_dir;


    while ((c = getopt_long(argc, argv, "i:o:", long_opts, NULL)) != -1)
    {
        switch (c)
        {
            case 'i':
                strncpy(input_dir, optarg, MAXPATHLEN);
                realpath(input_dir, sd_dir.src);
                strncat(sd_dir.src, "/", MAXPATHLEN-1);
                break;
            case 'o':
                strncpy(output_dir, optarg, MAXPATHLEN);
                realpath(output_dir, sd_dir.dest);
                strncat(sd_dir.dest, "/", MAXPATHLEN-1);
                break;
            case 'p':
                strncpy(prefix, optarg, MAXFILENAME);
                break;
            default:
                usage(EXIT_FAILURE);
        }
    }

    PrintSrcDestFile(&sd_dir);

    struct srcDestFile sd_file_list[MAXDIRCAPACITY];

    if(!sd_GetFilesFromDir(sd_dir, sd_file_list)){
        fprintf(stderr, "Issue opening up directory!\n");
        return 1;
    }

    if (strcmp(prefix, "") == 0){
        if(sd_NewFileNames(sd_file_list, sd_dir.dest) != NULL){
            printf("New names created\n");
        }
    } else {
        if(sd_NewFileNames(sd_file_list, prefix) != NULL){
            printf("New names created\n");
        }

    }

    PrependAbsPath(sd_dir, sd_file_list);

    sd_CopyFiles(sd_file_list);
}

