#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int check(char **directoryNames, int directorylength, char *filename)
{
    int check = 0;
        for(int i = 0; i < directorylength; i++)
        {
            if(strcasecmp(directoryNames[i], filename) == 0)
            {
                return 1;
            }
        }
    return 0;
}

void recurseDirectories(char *path, char *search, int len) {
    DIR *dir;
    struct dirent *dirp;

    if ((dir = opendir(path)) == NULL) {
        printf("Cannot open directory.");
        exit(0);
    }

    char **filesNames = malloc(sizeof(char *));
    int filesLength = 0;

    char **dirNames = malloc(sizeof(char *));
    int dirNamesLength = 0;


    while ((dirp = readdir(dir)) != NULL) {
        if (dirp->d_name[0] == '.') continue;

        if (dirp->d_type == DT_DIR) {
            dirNames = realloc(dirNames, sizeof(char *) * (dirNamesLength + 1));
            dirNames[dirNamesLength] = malloc(sizeof(char) * 256);
            strcpy(dirNames[dirNamesLength], dirp->d_name);
            dirNamesLength++;
        }
        filesNames = realloc(filesNames, sizeof(char *) * (filesLength + 1));
        filesNames[filesLength] = malloc(1012 * sizeof(char));
        strcpy(filesNames[filesLength], dirp->d_name);
        filesLength++;
    }

    for (int i = 0; i < filesLength; i++) {
        for (int j = i + 1; j < filesLength; j++) {
            if (strcasecmp(filesNames[i], filesNames[j]) > 0) {
                char *temp = filesNames[i];
                filesNames[i] = filesNames[j];
                filesNames[j] = temp;
            }
        }
    }

    for(int x = 0; x < filesLength; x++)
    {
        for (int i = 0; i < len; i++) {
            printf("  ");
        }
        printf("- %s", filesNames[x]);
        if (check(dirNames, dirNamesLength, filesNames[x]) == 1) {
            printf("/");
        }
        printf("\n");

        if (check(dirNames, dirNamesLength, filesNames[x]) == 1) {
            char newPath[256];
            strcpy(newPath, path);
            strcat(newPath, "/");
            strcat(newPath, filesNames[x]);
            recurseDirectories(newPath, search, len + 1);
        }
        free(filesNames[x]);
    }
    for(int y = 0; y < dirNamesLength; y++)
        free(dirNames[y]);
    free(dirNames);
    free(dirp);
    free(filesNames);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        printf("Usage: ./tree\n");
        printf("NOTHING ELSE, no funny business.\n");
        exit(1);
    }
    printf(".\n");
    recurseDirectories(".", argv[1], 1);
    return 0;
}