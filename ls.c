#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *dirp;
    int isLong = 0;
    struct stat buf;

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
    char **filesNames = malloc(sizeof(char *));
    int filesLength = 0;

    while ((dirp = readdir(dir)) != NULL) {
        if (!strcmp(dirp->d_name, "..") || !strcasecmp(dirp->d_name, ".")) continue;

        files[filesLength] = malloc(1012 * sizeof(char));

        if (isLong == 1) {
            stat(dirp->d_name, &buf);
            char *time = ctime(&buf.st_mtime);
            time[strlen(time) - 1] = '\0';
            char *permissions = malloc(11 * sizeof(char));
            permissions[0] = (S_ISDIR(buf.st_mode)) ? 'd' : '-';
            permissions[1] = (buf.st_mode & S_IRUSR) ? 'r' : '-';
            permissions[2] = (buf.st_mode & S_IWUSR) ? 'w' : '-';
            permissions[3] = (buf.st_mode & S_IXUSR) ? 'x' : '-';
            permissions[4] = (buf.st_mode & S_IRGRP) ? 'r' : '-';
            permissions[5] = (buf.st_mode & S_IWGRP) ? 'w' : '-';
            permissions[6] = (buf.st_mode & S_IXGRP) ? 'x' : '-';
            permissions[7] = (buf.st_mode & S_IROTH) ? 'r' : '-';
            permissions[8] = (buf.st_mode & S_IWOTH) ? 'w' : '-';
            permissions[9] = (buf.st_mode & S_IXOTH) ? 'x' : '-';
            permissions[10] = '\0';
            char *group = getgrgid(buf.st_gid)->gr_name;
            char *user = getpwuid(buf.st_uid)->pw_name;
            sprintf(files[filesLength], "%s\t%s\t%s\t%lld\t%s\t%s", permissions, user, group, buf.st_size, time, dirp->d_name);
            free(permissions);
            filesNames[filesLength] = malloc(1012 * sizeof(char));
            strcpy(filesNames[filesLength], dirp->d_name);
            filesNames = realloc(filesNames, sizeof(char *) * (filesLength + 2));
        } else strcpy(files[filesLength], dirp->d_name);

        filesLength++;
        files = realloc(files, sizeof(char *) * (filesLength + 1));
    }

    if (isLong == 1) {
        for (int i = 0; i < filesLength; i++) {
            for (int j = i + 1; j < filesLength; j++) {
                if (strcmp(filesNames[i], filesNames[j]) > 0) {
                    char *temp = files[i];
                    files[i] = files[j];
                    files[j] = temp;
                    temp = filesNames[i];
                    filesNames[i] = filesNames[j];
                    filesNames[j] = temp;
                }
            }
        }
    } else {
        for (int i = 0; i < filesLength; i++) {
            for (int j = i + 1; j < filesLength; j++) {
                if (strcasecmp(files[i], files[j]) > 0) {
                    char *temp = files[i];
                    files[i] = files[j];
                    files[j] = temp;
                }
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