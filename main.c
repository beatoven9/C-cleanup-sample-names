#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define MAXPATHLEN 200
#define MAXDIRCAPACITY 500

char* getFilesFromDir(char *dirToOpen, char filesList[500][255]);
void printStringsInArray(char myArray[500][255]);

int main(int argc, char *argv[]) {
    if ( argc == 2){
        printf("The argument supplied is %s\n", argv[1]);
    } else if( argc > 2 ){
        printf("Too many args supplied\n");
    } else {
        printf("One argument expected\n");
    }

    char filesList[MAXDIRCAPACITY][MAXNAMLEN];

    char* filesPtr = getFilesFromDir(argv[1], filesList);

    printStringsInArray(filesList);
}

char* getFilesFromDir(char *dirToOpen, char filesList[500][255]){
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

void printStringsInArray(char myArray[500][255]){ 
    int x = 0;
    while(strcmp(myArray[x], "\0") != 0){
        printf("%s\n", myArray[x]);
        x++;
    }

}
