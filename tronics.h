#ifndef TRONICS_H
#define TRONICS_H

void init_repo(const char *repo_name);
void add_file(const char *filename);
void commit(const char *message);
void show_log();
void simulate_iot();
void simulate_cad();
void create_branch(const char *name);
void list_branches();
void checkout_branch(const char *name);
void diff_file(const char *filename);
void restore_file(const char *filename);
void status(); // newly added

#endif
