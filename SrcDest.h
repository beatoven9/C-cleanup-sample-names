#include "Constants.h"

#ifndef SRCDEST
#define SRCDEST

struct srcDestDir {
    char src[MAXPATHLEN];
    char dest[MAXPATHLEN];
};

struct srcDestFile {
    char src[MAXPATHLEN];
    char dest[MAXPATHLEN];
};

struct srcDestFile* sd_NewFileNames(struct srcDestFile sd_list[]);
char* NewFileNames(char oldFilesList[MAXDIRCAPACITY][MAXFILENAME], char newFilesList[MAXDIRCAPACITY][MAXFILENAME]);
struct srcDestFile* sd_GetFilesFromDir(struct srcDestFile sd_dir, struct srcDestFile sd_file_list[MAXFILENAME]);
char* GetFilesFromDir(char *dirToOpen, char filesList[MAXDIRCAPACITY][MAXFILENAME]);
void CopyFileTo(char *src, char *dest);
void BuiltInCopy(struct srcDestFile sd);
struct srcDestFile* GetAbsPaths(struct srcDestFile *sd);
void sd_PrintDestinationFiles(struct srcDestFile sd_file_list[]);

void PrintSrcDestDir(struct srcDestDir *sd);
void PrintSrcDestFile(struct srcDestFile *sd);

#endif
