// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <time.h>
// #include <dirent.h>
// #include <errno.h>

// #define MAX_FILES 256

// int is_meta_file(const char *filename) {
//     return strstr(filename, ".meta") != NULL;
// }

// // Create directory (fixed to handle nested paths)
// void create_dir(const char *path) {
//     char tmp[512];
//     strncpy(tmp, path, sizeof(tmp));
//     tmp[sizeof(tmp) - 1] = '\0';
//     for (char *p = tmp + 1; *p; p++) {
//         if (*p == '/') {
//             *p = '\0';
//             mkdir(tmp, 0755);
//             *p = '/';
//         }
//     }
//     if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
//         perror("Error creating directory");
//     }
// }

// // Copy file (unchanged)
// void copy_file(const char *src, const char *dest) {
//     FILE *fsrc = fopen(src, "rb");
//     if (!fsrc) {
//         perror("Error opening source file");
//         return;
//     }
//     FILE *fdest = fopen(dest, "wb");
//     if (!fdest) {
//         perror("Error opening destination file");
//         fclose(fsrc);
//         return;
//     }
//     char buffer[1024];
//     size_t n;
//     while ((n = fread(buffer, 1, sizeof(buffer), fsrc)) > 0) {
//         fwrite(buffer, 1, n, fdest);
//     }
//     fclose(fsrc);
//     fclose(fdest);
// }

// // Copy staged files (enhanced with debugging)
// void copy_staged_files(const char *dest_dir) {
//     DIR *dir = opendir(".tronics/stage");
//     if (!dir) {
//         perror("Error opening .tronics/stage");
//         return;
//     }

//     struct dirent *entry;
//     char src[512], dest[512];
//     int has_files = 0;
//     while ((entry = readdir(dir)) != NULL) {
//         if (entry->d_type == DT_REG) {
//             snprintf(src, sizeof(src), ".tronics/stage/%s", entry->d_name);
//             snprintf(dest, sizeof(dest), "%s/%s", dest_dir, entry->d_name);
//             copy_file(src, dest);
//             has_files = 1;
//         }
//     }
//     closedir(dir);
//     if (!has_files) {
//         printf("Debug: No files found in .tronics/stage\n");
//     }
// }

// // Initialize repository (unchanged)
// void init_repo(const char *repo_name) {
//     char path[256];

//     sprintf(path, "%s", repo_name);
//     create_dir(path);

//     sprintf(path, "%s/.tronics", repo_name);
//     create_dir(path);

//     sprintf(path, "%s/.tronics/objects", repo_name);
//     create_dir(path);

//     sprintf(path, "%s/.tronics/refs", repo_name);
//     create_dir(path);

//     sprintf(path, "%s/.tronics/stash", repo_name);
//     create_dir(path);

//     sprintf(path, "%s/.tronics/stage", repo_name);
//     create_dir(path);

//     sprintf(path, "%s/.tronics/tags", repo_name);
//     create_dir(path);

//     sprintf(path, "%s/.tronics/history", repo_name);
//     create_dir(path);

//     sprintf(path, "%s/.tronics/log.txt", repo_name);
//     FILE *logf = fopen(path, "w");
//     fclose(logf);

//     sprintf(path, "%s/.tronics/stash/log.txt", repo_name);
//     logf = fopen(path, "w");
//     fclose(logf);

//     sprintf(path, "%s/.tronics/config", repo_name);
//     FILE *f = fopen(path, "w");
//     if (f) {
//         fprintf(f, "{ \"branch\": \"main\", \"version\": \"0.3.1\" }\n");
//         fclose(f);
//     }

//     sprintf(path, "%s/.tronics/refs/main", repo_name);
//     f = fopen(path, "w");
//     if (f) {
//         fprintf(f, "Branch main created.\n");
//         fclose(f);
//     }

//     printf("Initialized empty Tronics repository in %s/.tronics\n", repo_name);
// }

// // Add file (unchanged)
// void add_file(const char *filename) {
//     char dest[512];
//     snprintf(dest, sizeof(dest), ".tronics/stage/%s", filename);
//     copy_file(filename, dest);
//     printf("Staged: %s → .tronics/stage/\n", filename);
// }

// // Get current branch (unchanged)
// char *get_current_branch() {
//     static char branch[128];
//     FILE *config = fopen(".tronics/config", "r");
//     if (config) {
//         char line[256];
//         if (fgets(line, sizeof(line), config)) {
//             char *b = strstr(line, "\"branch\": \"");
//             if (b) {
//                 b += 11;
//                 char *end = strchr(b, '"');
//                 if (end) {
//                     *end = '\0';
//                     strncpy(branch, b, sizeof(branch));
//                 }
//             }
//         }
//         fclose(config);
//     } else {
//         strcpy(branch, "unknown");
//     }
//     return branch;
// }

// // Commit function (enhanced with debugging)
// void commit(const char *message) {
//     time_t now = time(NULL);
//     char timestamp[64];
//     strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
//     usleep(100000); // Ensure unique timestamps

//     char *branch = get_current_branch();
//     char commit_dir[256];
//     snprintf(commit_dir, sizeof(commit_dir), ".tronics/objects/%s/%s", branch, timestamp);
//     create_dir(commit_dir);

//     copy_staged_files(commit_dir);

//     // Verify copied files
//     DIR *commit_dir_check = opendir(commit_dir);
//     if (commit_dir_check) {
//         struct dirent *entry;
//         int has_files = 0;
//         while ((entry = readdir(commit_dir_check)) != NULL) {
//             if (entry->d_type == DT_REG) {
//                 has_files = 1;
//             }
//         }
//         closedir(commit_dir_check);
//         if (!has_files) {
//             printf("Debug: No files found in commit dir %s\n", commit_dir);
//         }
//     } else {
//         perror("Error opening commit directory for verification");
//     }

//     DIR *dir = opendir(".tronics/stage");
//     if (dir) {
//         struct dirent *entry;
//         while ((entry = readdir(dir)) != NULL) {
//             if (entry->d_type == DT_REG && !is_meta_file(entry->d_name)) {
//                 char history_path[512];
//                 snprintf(history_path, sizeof(history_path), ".tronics/history/%s", entry->d_name);
//                 create_dir(history_path); // Ensure parent directory exists
//                 FILE *hf = fopen(history_path, "a");
//                 if (hf) {
//                     fprintf(hf, "[%s] %s (%s)\n", timestamp, message, branch);
//                     fclose(hf);
//                 }
//             }
//         }
//         closedir(dir);
//     }

//     FILE *logf = fopen(".tronics/log.txt", "a");
//     if (logf) {
//         fprintf(logf, "[%s] Commit: %s (%s)\n", timestamp, message, branch);
//         fclose(logf);
//     }

//     system("rm -f .tronics/stage/*");
//     printf("Committed: \"%s\"\n", message);
// }

// // Status function (simplified with debugging)
// void status(void) {
//     char *current_branch = get_current_branch();
//     printf("== Tronics Status ==\n");

//     // Staged files
//     printf("Staged files:\n");
//     DIR *stage_dir = opendir(".tronics/stage");
//     int has_staged = 0;
//     if (stage_dir) {
//         struct dirent *entry;
//         while ((entry = readdir(stage_dir)) != NULL) {
//             if (entry->d_type == DT_REG && !is_meta_file(entry->d_name)) {
//                 printf("  %s\n", entry->d_name);
//                 has_staged = 1;
//             }
//         }
//         closedir(stage_dir);
//     } else {
//         perror("Debug: Failed to open .tronics/stage");
//     }
//     if (!has_staged) {
//         printf("  (none)\n");
//     }

//     // Committed files
//     printf("Committed files:\n");
//     char commit_path[256];
//     snprintf(commit_path, sizeof(commit_path), ".tronics/objects/%s", current_branch);
//     DIR *branch_dir = opendir(commit_path);
//     int has_committed = 0;
//     if (branch_dir) {
//         struct dirent *commit_entry;
//         while ((commit_entry = readdir(branch_dir)) != NULL) {
//             if (commit_entry->d_type == DT_DIR && strcmp(commit_entry->d_name, ".") != 0 && strcmp(commit_entry->d_name, "..") != 0) {
//                 char timestamp_path[512];
//                 snprintf(timestamp_path, sizeof(timestamp_path), "%s/%s", commit_path, commit_entry->d_name);
//                 DIR *timestamp_dir = opendir(timestamp_path);
//                 if (timestamp_dir) {
//                     struct dirent *file_entry;
//                     while ((file_entry = readdir(timestamp_dir)) != NULL) {
//                         if (file_entry->d_type == DT_REG && !is_meta_file(file_entry->d_name)) {
//                             printf("  %s\n", file_entry->d_name);
//                             has_committed = 1;
//                         }
//                     }
//                     closedir(timestamp_dir);
//                 } else {
//                     perror("Debug: Failed to open timestamp dir");
//                     printf("Debug: Path was %s\n", timestamp_path);
//                 }
//             }
//         }
//         closedir(branch_dir);
//     } else {
//         perror("Debug: Failed to open commit path");
//         printf("Debug: Path was %s\n", commit_path);
//     }
//     if (!has_committed) {
//         printf("  (none)\n");
//     }

//     printf("Current branch:\n  %s\n", current_branch);
// }

// // Show log (unchanged)
// void show_log() {
//     FILE *f = fopen(".tronics/log.txt", "r");
//     if (!f) {
//         printf("No commits yet.\n");
//         return;
//     }

//     char line[256];
//     while (fgets(line, sizeof(line), f)) {
//         line[strcspn(line, "\n")] = 0;
//         printf("%s\n", line);
//     }
//     fclose(f);
// }

// // Simulate IoT (fixed to use branch-specific path)
// void simulate_iot() {
//     char *branch = get_current_branch();
//     char timestamp[64];
//     time_t now = time(NULL);
//     strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
//     char path[512];
//     snprintf(path, sizeof(path), ".tronics/objects/%s/%s", branch, timestamp);
//     create_dir(path);
//     snprintf(path, sizeof(path), ".tronics/objects/%s/%s/sensor_data.txt", branch, timestamp);
//     FILE *f = fopen(path, "a");
//     if (f) {
//         fprintf(f, "Timestamp: %ld | Temp: 24.2°C | Pressure: 1015 hPa\n", now);
//         fclose(f);
//         printf("IIoT data appended to sensor_data.txt\n");
//     } else {
//         perror("Error: Failed to write sensor_data.txt");
//     }
// }

// // Simulate CAD (fixed to use branch-specific path)
// void simulate_cad() {
//     char *branch = get_current_branch();
//     char timestamp[64];
//     time_t now = time(NULL);
//     strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
//     char path[512];
//     snprintf(path, sizeof(path), ".tronics/objects/%s/%s", branch, timestamp);
//     create_dir(path);
//     snprintf(path, sizeof(path), ".tronics/objects/%s/%s/cad_log.txt", branch, timestamp);
//     FILE *f = fopen(path, "a");
//     if (f) {
//         fprintf(f, "CAD event: draw_line from (0,0) to (100,200)\n");
//         fclose(f);
//         printf("CAD event written to cad_log.txt\n");
//     } else {
//         perror("Error: Failed to write cad_log.txt");
//     }
// }

// // Diff file (unchanged)
// void diff_file(const char *filename) {
//     char staged_path[256];
//     snprintf(staged_path, sizeof(staged_path), ".tronics/stage/%s", filename);

//     FILE *f1 = fopen(filename, "r");
//     FILE *f2 = fopen(staged_path, "r");

//     if (!f1 || !f2) {
//         printf("Either working file or staged file is missing.\n");
//         if (f1) fclose(f1);
//         if (f2) fclose(f2);
//         return;
//     }

//     printf("Diff (%s):\n", filename);
//     char line1[256], line2[256];
//     int line_num = 1;
//     while (fgets(line1, sizeof(line1), f1) && fgets(line2, sizeof(line2), f2)) {
//         line1[strcspn(line1, "\n")] = 0;
//         line2[strcspn(line2, "\n")] = 0;
//         if (strcmp(line1, line2) != 0) {
//             printf("Line %d:\n  Working: %s\n  Staged: %s\n", line_num, line1, line2);
//         }
//         line_num++;
//     }
//     fclose(f1);
//     fclose(f2);
// }

// // Restore file (unchanged)
// void restore_file(const char *filename) {
//     char object_path[256];
//     snprintf(object_path, sizeof(object_path), ".tronics/objects/%s", filename);
//     copy_file(object_path, filename);
//     printf("Restored %s from last commit.\n", filename);
// }

// // Create branch (unchanged)
// void create_branch(const char *name) {
//     char path[128];
//     snprintf(path, sizeof(path), ".tronics/refs/%s", name);
//     FILE *f = fopen(path, "w");
//     if (f) {
//         fprintf(f, "Branch %s created.\n", name);
//         fclose(f);
//         printf("Created branch: %s\n", name);
//     } else {
//         printf("Failed to create branch: %s\n", name);
//     }
// }

// // List branches (unchanged)
// void list_branches() {
//     printf("Available branches:\n");
//     system("ls .tronics/refs 2>/dev/null || echo '(none)'");
// }

// // Checkout branch (unchanged)
// void checkout_branch(const char *name) {
//     char path[128];
//     snprintf(path, sizeof(path), ".tronics/refs/%s", name);
//     if (access(path, F_OK) != 0) {
//         printf("Branch %s does not exist.\n", name);
//         return;
//     }

//     FILE *config = fopen(".tronics/config", "w");
//     if (config) {
//         fprintf(config, "{ \"branch\": \"%s\", \"version\": \"0.3.1\" }\n", name);
//         fclose(config);
//         printf("Switched to branch: %s\n", name);
//     } else {
//         printf("Failed to switch to branch: %s\n", name);
//     }
// }

// // Get latest commit timestamp for a branch (unchanged)
// char *get_latest_commit(const char *branch) {
//     static char timestamp[64] = "";
//     char dir_path[256];
//     snprintf(dir_path, sizeof(dir_path), ".tronics/objects/%s", branch);
//     DIR *dir = opendir(dir_path);
//     if (!dir) {
//         return timestamp;
//     }

//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//             if (strcmp(entry->d_name, timestamp) > 0) {
//                 strncpy(timestamp, entry->d_name, sizeof(timestamp));
//             }
//         }
//     }
//     closedir(dir);
//     return timestamp;
// }

// // Merge branch (unchanged)
// void merge_branch(const char *branch_name) {
//     char *current_branch = get_current_branch();
//     if (strcmp(current_branch, branch_name) == 0) {
//         printf("Cannot merge branch %s into itself.\n", branch_name);
//         return;
//     }

//     char branch_path[256];
//     snprintf(branch_path, sizeof(branch_path), ".tronics/refs/%s", branch_name);
//     if (access(branch_path, F_OK) != 0) {
//         printf("Branch %s does not exist.\n", branch_name);
//         return;
//     }

//     time_t now = time(NULL);
//     char timestamp[64];
//     strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
//     char commit_dir[256];
//     snprintf(commit_dir, sizeof(commit_dir), ".tronics/objects/%s/%s", current_branch, timestamp);
//     create_dir(commit_dir);

//     char *source_timestamp = get_latest_commit(branch_name);
//     if (strlen(source_timestamp) == 0) {
//         printf("Error: No commits found in branch %s\n", branch_name);
//         return;
//     }
//     char command[512];
//     snprintf(command, sizeof(command), "cp .tronics/objects/%s/%s/* %s/ 2>/dev/null", branch_name, source_timestamp, commit_dir);
//     system(command);

//     DIR *dir = opendir(commit_dir);
//     if (!dir) {
//         printf("Error: Could not open commit directory %s\n", commit_dir);
//         return;
//     }

//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (entry->d_type == DT_REG && strstr(entry->d_name, ".stl")) {
//             char current_path[512], source_path[512];
//             char *current_timestamp = get_latest_commit(current_branch);
//             if (strlen(current_timestamp) == 0) {
//                 printf("Warning: No prior commits in %s, skipping conflict check for %s\n", current_branch, entry->d_name);
//                 continue;
//             }
//             snprintf(current_path, sizeof(current_path), ".tronics/objects/%s/%s/%s.meta", current_branch, current_timestamp, entry->d_name);
//             snprintf(source_path, sizeof(source_path), ".tronics/objects/%s/%s/%s.meta", branch_name, source_timestamp, entry->d_name);

//             if (access(current_path, F_OK) == 0 && access(source_path, F_OK) == 0) {
//                 FILE *f1 = fopen(current_path, "r");
//                 FILE *f2 = fopen(source_path, "r");
//                 if (f1 && f2) {
//                     char line1[256], line2[256];
//                     int has_conflict = 0;
//                     while (fgets(line1, sizeof(line1), f1) && fgets(line2, sizeof(line2), f2)) {
//                         line1[strcspn(line1, "\n")] = 0;
//                         line2[strcspn(line2, "\n")] = 0;
//                         if (strcmp(line1, line2) != 0) {
//                             has_conflict = 1;
//                             break;
//                         }
//                     }
//                     if (has_conflict) {
//                         printf("CONFLICT: CAD file %s has metadata differences\n", entry->d_name);
//                     }
//                     fclose(f1);
//                     fclose(f2);
//                 } else {
//                     if (f1) fclose(f1);
//                     if (f2) fclose(f2);
//                     printf("Warning: Could not open metadata files for %s\n", entry->d_name);
//                 }
//             } else {
//                 printf("Warning: Metadata file missing for %s in one or both branches\n", entry->d_name);
//             }
//         }
//     }
//     closedir(dir);

//     FILE *logf = fopen(".tronics/log.txt", "a");
//     if (logf) {
//         fprintf(logf, "[%s] Merge: Merged branch %s into %s\n", timestamp, branch_name, current_branch);
//         fclose(logf);
//     }

//     printf("Merged branch %s into current branch\n", branch_name);
// }

// // Show file history (unchanged)
// void show_file_history(const char *filename) {
//     char history_path[512];
//     snprintf(history_path, sizeof(history_path), ".tronics/history/%s", filename);

//     FILE *f = fopen(history_path, "r");
//     if (!f) {
//         printf("No history for %s\n", filename);
//         return;
//     }

//     printf("History for %s:\n", filename);
//     char line[256];
//     while (fgets(line, sizeof(line), f)) {
//         line[strcspn(line, "\n")] = 0;
//         printf("%s\n", line);
//     }
//     fclose(f);
// }

// // Stash push (unchanged)
// void stash_push() {
//     time_t now = time(NULL);
//     char timestamp[64];
//     strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));

//     char stash_dir[256];
//     snprintf(stash_dir, sizeof(stash_dir), ".tronics/stash/%s", timestamp);
//     create_dir(stash_dir);

//     char command[512];
//     snprintf(command, sizeof(command), "cp .tronics/stage/* %s/ 2>/dev/null", stash_dir);
//     system(command);

//     FILE *logf = fopen(".tronics/stash/log.txt", "a");
//     if (logf) {
//         fprintf(logf, "[%s] Stashed changes\n", timestamp);
//         fclose(logf);
//     }

//     system("rm -f .tronics/stage/*");
// }

// // Stash pop (unchanged)
// void stash_pop() {
//     DIR *dir = opendir(".tronics/stash");
//     if (!dir) {
//         printf("No stashed changes found.\n");
//         return;
//     }

//     char latest_stash[64] = "";
//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//             if (strcmp(entry->d_name, latest_stash) > 0) {
//                 strcpy(latest_stash, entry->d_name);
//             }
//         }
//     }
//     closedir(dir);

//     if (strlen(latest_stash) == 0) {
//         printf("No stashed changes found.\n");
//         return;
//     }

//     char command[512];
//     snprintf(command, sizeof(command), "cp .tronics/stash/%s/* .tronics/stage/ 2>/dev/null", latest_stash);
//     system(command);

//     snprintf(command, sizeof(command), "rm -rf .tronics/stash/%s", latest_stash);
//     system(command);

// }

// // Stash list (unchanged)
// void stash_list() {
//     printf("Stashed changes:\n");
//     FILE *f = fopen(".tronics/stash/log.txt", "r");
//     if (!f) {
//         printf("(none)\n");
//         return;
//     }

//     char line[256];
//     while (fgets(line, sizeof(line), f)) {
//         line[strcspn(line, "\n")] = 0;
//         printf("%s\n", line);
//     }
//     fclose(f);
// }

// // Create tag (unchanged)
// void create_tag(const char *tag_name) {
//     char path[128];
//     snprintf(path, sizeof(path), ".tronics/tags/%s", tag_name);
//     FILE *f = fopen(path, "w");
//     if (f) {
//         time_t now = time(NULL);
//         char timestamp[64];
//         strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
//         fprintf(f, "Tag %s created at %s\n", tag_name, timestamp);
//         fclose(f);
//         printf("Created tag: %s\n", tag_name);
//     } else {
//         printf("Failed to create tag: %s\n", tag_name);
//     }
// }

// // List tags (unchanged)
// void list_tags() {
//     printf("Available tags:\n");
//     system("ls .tronics/tags 2>/dev/null || echo '(none)'");
// }

// // Parse STL file for vertex count (unchanged)
// int parse_stl_vertices(const char *filename) {
//     FILE *f = fopen(filename, "r");
//     if (!f) {
//         perror("Error opening STL file");
//         return -1;
//     }

//     int vertex_count = 0;
//     char line[256];
//     int is_ascii = 0;

//     if (fgets(line, sizeof(line), f)) {
//         if (strncmp(line, "solid ", 6) == 0) {
//             is_ascii = 1;
//         }
//     }
//     rewind(f);

//     if (is_ascii) {
//         while (fgets(line, sizeof(line), f)) {
//             if (strstr(line, "vertex ")) {
//                 vertex_count++;
//             }
//         }
//     } else {
//         fseek(f, 84, SEEK_SET);
//         u_int32_t num_triangles;
//         if (fread(&num_triangles, sizeof(u_int32_t), 1, f) == 1) {
//             vertex_count = num_triangles * 3;
//         }
//     }

//     fclose(f);
//     return vertex_count > 0 ? vertex_count : -1;
// }

// // Add CAD file with metadata (unchanged)
// void add_cad(const char *filename) {
//     char dest[512];
//     snprintf(dest, sizeof(dest), ".tronics/stage/%s", filename);
//     copy_file(filename, dest);

//     char meta_path[512];
//     snprintf(meta_path, sizeof(meta_path), ".tronics/stage/%s.meta", filename);
//     FILE *meta = fopen(meta_path, "w");
//     if (meta) {
//         struct stat st;
//         if (stat(filename, &st) == 0) {
//             int vertices = parse_stl_vertices(filename);
//             fprintf(meta, "size:%ld\nvertices:%d\n", st.st_size, vertices >= 0 ? vertices : 0);
//             fclose(meta);
//             printf("Staged CAD: %s → .tronics/stage/\n", filename);
//         } else {
//             perror("Error getting file stats");
//             fclose(meta);
//         }
//     } else {
//         perror("Error creating metadata file");
//     }
// }

// // Diff CAD file metadata (unchanged)
// void diff_cad(const char *filename) {
//     char staged_path[512], staged_meta[512];
//     snprintf(staged_path, sizeof(staged_path), ".tronics/stage/%s", filename);
//     snprintf(staged_meta, sizeof(staged_meta), ".tronics/stage/%s.meta", filename);

//     FILE *f1 = fopen(filename, "rb");
//     FILE *f2 = fopen(staged_path, "rb");
//     FILE *meta = fopen(staged_meta, "r");

//     if (!f1 || !f2 || !meta) {
//         printf("Either working file, staged file, or metadata is missing.\n");
//         if (f1) fclose(f1);
//         if (f2) fclose(f2);
//         if (meta) fclose(meta);
//         return;
//     }

//     printf("Diff CAD (%s):\n", filename);

//     struct stat st1, st2;
//     if (stat(filename, &st1) == 0 && stat(staged_path, &st2) == 0) {
//         if (st1.st_size != st2.st_size) {
//             printf("  Size: %ld (working) vs %ld (staged)\n", st1.st_size, st2.st_size);
//         } else {
//             printf("  Size: %ld (unchanged)\n", st1.st_size);
//         }
//     }

//     char line[256];
//     while (fgets(line, sizeof(line), meta)) {
//         line[strcspn(line, "\n")] = 0;
//         printf("  Metadata: %s\n", line);
//     }

//     fclose(f1);
//     fclose(f2);
//     fclose(meta);
// }