#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "tics.h"

int is_meta_file(const char *filename);
void create_dir(const char *path);
void copy_file(const char *src, const char *dest);

#endif