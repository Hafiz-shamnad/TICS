#ifndef TRONICS_H
#define TRONICS_H

// Function Prototypes (unchanged)
void create_dir(const char *path);
void copy_file(const char *src, const char *dest);
void init_repo(const char *repo_name);
void add_file(const char *filename);
void commit(const char *message);
void show_log();
void simulate_iot();
void simulate_cad();
void diff_file(const char *filename);
void restore_file(const char *filename);
void status();
void create_branch(const char *name);
void list_branches();
void checkout_branch(const char *name);
void merge_branch(const char *branch_name);
void show_file_history(const char *filename);
void stash_push();
void stash_pop();
void stash_list();
void create_tag(const char *tag_name);
void list_tags();
void diff_cad(const char *filename);
void add_cad(const char *filename);
int parse_stl_vertices(const char *filename);
char *get_latest_commit(const char *branch);
int is_meta_file(const char *filename);
int is_duplicate(const char *filename, char listed[][256], int count);


#endif
