#include "file_ops.h"

int is_meta_file(const char *filename) {
    return strstr(filename, ".meta") != NULL;
}

void create_dir(const char *path) {
    char tmp[MAX_PATH];
    strncpy(tmp, path, sizeof(tmp));
    tmp[sizeof(tmp) - 1] = '\0';
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
    if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
        perror("Error creating directory");
    }
}

void copy_file(const char *src, const char *dest) {
    FILE *fsrc = fopen(src, "rb");
    if (!fsrc) {
        perror("Error opening source file");
        return;
    }
    FILE *fdest = fopen(dest, "wb");
    if (!fdest) {
        perror("Error opening destination file");
        fclose(fsrc);
        return;
    }
    char buffer[1024];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), fsrc)) > 0) {
        fwrite(buffer, 1, n, fdest);
    }
    fclose(fsrc);
    fclose(fdest);
}