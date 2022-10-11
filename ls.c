#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *dirp;
    char *path = ".";

    if (argc > 1) {
        path = argv[1];
    }

    if ((dir = opendir(path)) == NULL) {
        printf("Error opening directory");
        exit(1);
    }

    char **files = malloc(sizeof(char *));
    int filesLength = 0;

    while ((dirp = readdir(dir)) != NULL) {
        files[filesLength] = malloc(strlen(dirp->d_name) + 1);
        strcpy(files[filesLength], dirp->d_name);
        filesLength++;
        files = realloc(files, sizeof(char *) * (filesLength + 1));

    }

    for (int i = 0; i < filesLength; i++) {
        for (int j = i + 1; j < filesLength; j++) {
            if (strcmp(files[i], files[j]) > 0) {
                char *temp = files[i];
                files[i] = files[j];
                files[j] = temp;
            }
        }
    }

    for (int i = 0; i < filesLength; i++) {
        printf("%s\n", files[i]);
    }
    closedir(dir);
    return 0;
}