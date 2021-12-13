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

struct srcDestFile* sd_NewFileNames(struct srcDestFile sd_list[], char *newFilePrefix);
struct srcDestFile* sd_GetFilesFromDir(struct srcDestFile sd_dir, struct srcDestFile sd_file_list[MAXFILENAME]);
struct srcDestFile* PrependAbsPath(struct srcDestFile absPath, struct srcDestFile *filesList);
void sd_PrintDestinationFiles(struct srcDestFile sd_file_list[]);
void sd_CopyFiles(struct srcDestFile *filesList);
void BuiltInCopy(struct srcDestFile sd);
void CopyFileTo(char *src, char *dest);

void PrintSrcDestDir(struct srcDestDir *sd);
void PrintSrcDestFile(struct srcDestFile *sd);

#endif
