#include "fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

typedef struct stat stat_t;

const char* load_file_null_terminated(const char* path) {
    stat_t st;
    if (stat(path, &st) != 0) {
        printf("%s\n", strerror(errno));
        return NULL;
    }

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("%s\n", strerror(errno));
        return NULL;
    }

    char* file_contents = malloc((st.st_size + 1) * sizeof(char));
    file_contents[st.st_size] = '\0';
    if (fread(file_contents, st.st_size, 1, file) != 1) {
        printf("%s\n", strerror(errno));
        fclose(file);
        free(file_contents);
        return NULL;
    }

    fclose(file);

    return file_contents;
}