#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h> // Checks for file type
#include <sys/stat.h> // for file statistics
#include <dirent.h> // for *DIR/dir information and struct
#include <time.h> // for ctime(), haven't really looked into this yet
#include <pwd.h> // To get user name
#include <grp.h> // To get group name

int main(int argc, char *argv[]) {
    // Using the presentation format for this
    // Create *dir pointer using dirent.h
    DIR *dir;
    struct dirent *dirp; // struct for directory information
    int isLong = 0;
    struct stat buf;

    // Check if -l flag is present
    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            isLong = 1;
        } else {
            printf("Usage: ls [-l]");
            exit(1);
        }
    }

    // Is direcotry not null?
    if ((dir = opendir(".")) == NULL) {
        printf("Error opening directory");
        exit(1);
    }

    // Char array for files, filesNames
    // Files will hold full file info, fileNames will hold just the file name if in -l mode
    char **files = malloc(sizeof(char *));
    char **filesNames = malloc(sizeof(char *));
    int filesLength = 0;

    // Untl we reach the end of the directory, read
    while ((dirp = readdir(dir)) != NULL) {
        // Ignore files that start with . (hidden files)
        if (dirp->d_name[0] == '.') continue;

        // Allocate 1012 bytes for full file length (random num to account for -l)
        // File names will be 256 bytes at most anyways
        files[filesLength] = malloc(1012 * sizeof(char));

        // If -l flag is present, get file info
        // Use stat to get file info, use ctime to format the unix epoc time
        if (isLong == 1) {

            // Get file info using stat
            stat(dirp->d_name, &buf);

            // Be careful, time has newline char at end
            char *time = ctime(&buf.st_mtime);
            time[strlen(time) - 1] = '\0';

            // Create a string for permissions of the file, use stats to make human readable
            char *permissions = malloc(11 * sizeof(char));

            // I don't understand bitwise operators -> https://codeforwin.org/2018/03/c-program-find-file-properties-using-stat-function.html 
            // Please help
            // Basically checks for permissions and sets the string to rwx or ---
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

            char *group = getgrgid(buf.st_gid)->gr_name; // Human readable group name
            char *user = getpwuid(buf.st_uid)->pw_name; // Human readable user name

            // buf.st_size only works with ld on ilab, but only lld on my computer
            // sprintf sends formatted string to specified buffer, so not printf (was getting confused earlier)
            sprintf(files[filesLength], "%s %s %s %ld\t%s %s", permissions, user, group, buf.st_size, time, dirp->d_name);
            free(permissions);

            // Remember to store fileNames as well for comparasion later on
            filesNames[filesLength] = malloc(1012 * sizeof(char));
            strcpy(filesNames[filesLength], dirp->d_name);
            filesNames = realloc(filesNames, sizeof(char *) * (filesLength + 2));
        } else strcpy(files[filesLength], dirp->d_name); // Basically don't do any of this shyte if -l flag is not present, simple strcpy

        filesLength++;
        files = realloc(files, sizeof(char *) * (filesLength + 1));
    }

    // If is long, sort files based on fileNames, not files
    // fileNames is ONLY for names, files MAY contain full length stuff
    if (isLong == 1) {
        for (int i = 0; i < filesLength; i++) {
            for (int j = i + 1; j < filesLength; j++) {
                if (strcasecmp(filesNames[i], filesNames[j]) > 0) { // Remember to use strcasecmp, not strcmp (am dum)
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

    // PRINT AND FREEEEEEEEEEE
    for (int i = 0; i < filesLength; i++) {
        printf("%s\n", files[i]);
        free(files[i]);
        if (isLong == 1) free(filesNames[i]);
    }
    free(files);
    free(dirp);
    free(filesNames); // Free fileNames even if not used bc of how it's declared, could change implementation but lazy
    closedir(dir);
    return 0;
}