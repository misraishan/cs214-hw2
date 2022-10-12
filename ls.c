#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *dirp;
    int isLong = 0;

    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            isLong = 1;
        } else {
            printf("Usage: ls [-l]");
            exit(1);
        }
    }

    if ((dir = opendir(".")) == NULL) {
        printf("Error opening directory");
        exit(1);
    }

    char **files = malloc(sizeof(char *));
    int filesLength = 0;

    while ((dirp = readdir(dir)) != NULL) {
        if (!strcmp(dirp->d_name, "..") || !strcasecmp(dirp->d_name, ".")) continue;
        if (isLong == 0) printf("Is long");

        files[filesLength] = malloc(strlen(dirp->d_name) + 1);
        strcpy(files[filesLength], dirp->d_name);
        filesLength++;
        files = realloc(files, sizeof(char *) * (filesLength + 1));
    }

    for (int i = 0; i < filesLength; i++) {
        for (int j = i + 1; j < filesLength; j++) {
            if (strcasecmp(files[i], files[j]) > 0) {
                char *temp = files[i];
                files[i] = files[j];
                files[j] = temp;
            }
        }
    }

    for (int i = 0; i < filesLength; i++) {
        printf("%s\n", files[i]);
        free(files[i]);
    }
    free(files);

    closedir(dir);
    return 0;
}