#ifndef COMMIT_OPS_H
#define COMMIT_OPS_H

#include "tics.h"

void copy_staged_files(const char *dest_dir);
void commit(const char *message);
void show_log(void);
void show_file_history(const char *filename);
void add_file(const char *filename);
void diff_file(const char *filename);
void restore_file(const char *filename);

#endif