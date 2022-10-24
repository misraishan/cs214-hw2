#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

void recurseDirectories(char *path, int step) {
    // Create *dir pointer using dirent.h
    DIR *dir;
    struct dirent *dirp; // struct for directory information

    // Is direcotry not null?
    if ((dir = opendir(path)) == NULL) {
        printf("Cannot open directory.");
        exit(0);
    }

    //fileNames will hold just the file name
    char **filesNames = malloc(sizeof(char *));
    int filesLength = 0;

    char **dirNames = malloc(sizeof(char *));
    int dirNamesLength = 0;

    

    // Untl we reach the end of the directory, read
        while ((dirp = readdir(dir)) != NULL) {
        // Ignore files that start with . (hidden files)
        if (dirp->d_name[0] == '.') continue;

        if (dirp->d_type == DT_DIR) {
            dirNames = realloc(dirNames, sizeof(char *) * (dirNamesLength + 1));
            dirNames[dirNamesLength] = malloc(sizeof(char) * 256);
            strcpy(dirNames[dirNamesLength], dirp->d_name);
            dirNamesLength++;
        }
        // Allocate 1012 bytes for full file length (random num to account for -l)
        // File names will be 256 bytes at most anyways
        filesNames[filesLength] = malloc(1012 * sizeof(char));
        strcpy(filesNames[filesLength], dirp->d_name); // Basically don't do any of this shyte if -l flag is not present, simple strcpy

        filesLength++;
        filesNames = realloc(filesNames, sizeof(char *) * (filesLength + 1));
    }

    //organize based on order
    for (int i = 0; i < filesLength; i++) {
        for (int j = i + 1; j < filesLength; j++) {
            if (strcasecmp(filesNames[i], filesNames[j]) > 0) {
                char *temp = filesNames[i];
                filesNames[i] = filesNames[j];
                filesNames[j] = temp;
            }
        }
    }
    
    // PRINT AND FREEEEEEEEEEE
    if(strcmp(path, "."))
            printf(".\n");
    for (int i = 0; i < filesLength; i++) {
        for(int x = 0; x < step; x++)
        {
            printf("  ");
        }
        printf("- %s\n", filesNames[i]);
        int check = 0;
        for(int y = 0; y < dirNamesLength; y++)
        {
            if(strcasecmp(dirNames[y], filesNames[i]))
            {
                check = 1;
                break;
            }
        }
        if(check == 1)
        {
            step+=1;
            recurseDirectories(filesNames[i], step);
        }
        free(filesNames[i]);
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
    recurseDirectories(".", 0);
    return 0;
}