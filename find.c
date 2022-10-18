#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void recurseDirectories(char *path, char *search) {
    DIR *dir;
    struct dirent *dirp;
    int mode = 0;

    if (strncmp(search, ".", 1) == 0) {
        mode = 1;
    }

    if ((dir = opendir(path)) == NULL) {
        printf("Cannot open directory.");
        exit(0);
    }

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

        // printf("Testing strncmp: %d", strncmp(dirp->d_name, search, 1));
        // printf("Testing strcmp: %d", strcmp(dirp->d_name, search));

        if (dirp->d_type != DT_DIR) {
            if (mode == 0 && strcmp(dirp->d_name, search) == 0) {
                printf("%s/%s\n", path, dirp->d_name);
            }
            if (mode == 1 && strstr(dirp->d_name, search) != 0) {
                printf("%s/%s\n", path, dirp->d_name);
            }
        }
    }

    closedir(dir);

    for (int i = 0; i < dirNamesLength; i++) {
        char *newPath = malloc(sizeof(char) * 256);
        strcpy(newPath, path);
        strcat(newPath, "/");
        strcat(newPath, dirNames[i]);
        free(dirNames[i]);
        recurseDirectories(newPath, search);
    }

    free(dirNames);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./ls <name>\n");
        exit(1);
    }
    recurseDirectories(".", argv[1]);
    return 0;
}
