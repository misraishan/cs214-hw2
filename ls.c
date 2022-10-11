#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *dir = popen("ls", "r");

    if (dir == NULL) {
        printf("Error, not a valid direcotry");
        exit(1);
    }

    // Variables for reading the directory
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Variables for storing file length and names
    char **files = malloc(sizeof(char *));
    int filesLength = 0;
    
    // Read the directory, store the file names
    while ((read = getline(&line, &len, dir)) != -1) {
        files[filesLength] = malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(files[filesLength], line);
        filesLength++;
        files = realloc(files, sizeof(char *) * (filesLength + 1));
    }

    // Print all the files
    for (int i = 0; i < filesLength; i++) {
        printf("%s", files[i]);
    }
    free(line);
    pclose(dir);
    exit(0);
}
