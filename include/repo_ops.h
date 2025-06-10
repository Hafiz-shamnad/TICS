#ifndef REPO_OPS_H
#define REPO_OPS_H

#include "tronics.h"

void init_repo(const char *repo_name);
void status(void);
char *get_current_branch(void);

#endif