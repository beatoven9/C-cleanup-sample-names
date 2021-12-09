#include "Constants.h"

struct srcDestDir {
    char src[MAXPATHLEN];
    char dest[MAXPATHLEN];
};

struct srcDestFile {
    char src[MAXPATHLEN];
    char dest[MAXPATHLEN];
};

char* NewFileNames(char oldFilesList[MAXDIRCAPACITY][MAXFILENAME], char newFilesList[MAXDIRCAPACITY][MAXFILENAME]);
char* GetFilesFromDir(char *dirToOpen, char filesList[MAXDIRCAPACITY][MAXFILENAME]);
void CopyFileTo(char *src, char *dest);
void BuiltInCopy(struct srcDestFile sd);
struct srcDestFile* GetAbsPaths(struct srcDestFile *sd);
