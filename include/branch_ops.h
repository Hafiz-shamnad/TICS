#ifndef BRANCH_OPS_H
#define BRANCH_OPS_H

#include "tronics.h"

void create_branch(const char *name);
void list_branches(void);
void checkout_branch(const char *name);
void merge_branch(const char *branch_name);
char *get_latest_commit(const char *branch);

#endif