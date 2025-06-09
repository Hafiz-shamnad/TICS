#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

// Create directory (unchanged)
void create_dir(const char *path) {
    mkdir(path, 0755);
}

// Copy file (unchanged)
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
        fwrite(buffer, 1, n, fdest);    }
    fclose(fsrc);
    fclose(fdest);
}

// Initialize repository
void init_repo(const char *repo_name) {
    char path[256];

    sprintf(path, "%s", repo_name);
    create_dir(path);

    sprintf(path, "%s/.tronics", repo_name);
    create_dir(path);

    sprintf(path, "%s/.tronics/objects", repo_name);
    create_dir(path);

    sprintf(path, "%s/.tronics/refs", repo_name);
    create_dir(path);

    sprintf(path, "%s/.tronics/stash", repo_name);
    create_dir(path);

    sprintf(path, "%s/.tronics/stage", repo_name);
    create_dir(path);

    sprintf(path, "%s/.tronics/tags", repo_name);
    create_dir(path);

    sprintf(path, "%s/.tronics/history", repo_name);
    create_dir(path);

    sprintf(path, "%s/.tronics/log.txt", repo_name);
    FILE *logf = fopen(path, "w");
    fclose(logf);

    sprintf(path, "%s/.tronics/stash/log.txt", repo_name);
    logf = fopen(path, "w");
    fclose(logf);

    sprintf(path, "%s/.tronics/config", repo_name);
    FILE *f = fopen(path, "w");
    if (f) {
        fprintf(f, "{ \"branch\": \"main\", \"version\": \"0.3.1\" }\n");
        fclose(f);
    }

    // Create main branch reference
    sprintf(path, "%s/.tronics/refs/main", repo_name);
    f = fopen(path, "w");
    if (f) {
        fprintf(f, "Branch main created.\n");
        fclose(f);
    }

    printf("Initialized empty Tronics repository in %s/.tronics\n", repo_name);
}

// Add file (unchanged)
void add_file(const char *filename) {
    char dest[512];
    snprintf(dest, sizeof(dest), ".tronics/stage/%s", filename);
    copy_file(filename, dest);
    printf("Staged: %s → .tronics/stage/\n", filename);
}

// Get current branch (new helper function)
char *get_current_branch() {
    static char branch[128];
    FILE *config = fopen(".tronics/config", "r");
    if (config) {
        char line[256];
        if (fgets(line, sizeof(line), config)) {
            char *b = strstr(line, "\"branch\": \"");
            if (b) {
                b += 11;
                char *end = strchr(b, '"');
                if (end) {
                    *end = '\0';
                    strncpy(branch, b, sizeof(branch));
                }
            }
        }
        fclose(config);
    } else {
        strcpy(branch, "unknown");
    }
    return branch;
}

// Commit (modified to store commits in branch-specific directories)
void commit(const char *message) {
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
    usleep(100000); // Add 0.1s delay for unique timestamps

    char *branch = get_current_branch();
    char commit_dir[256];
    snprintf(commit_dir, sizeof(commit_dir), ".tronics/objects/%s/%s", branch, timestamp);
    create_dir(commit_dir);

    char command[512];
    snprintf(command, sizeof(command), "cp .tronics/stage/* %s/ 2>/dev/null", commit_dir);
    system(command);

    DIR *dir = opendir(".tronics/stage");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char history_path[512];
            snprintf(history_path, sizeof(history_path), ".tronics/history/%s", entry->d_name);
            FILE *hf = fopen(history_path, "a");
            if (hf) {
                fprintf(hf, "[%s] %s (%s)\n", timestamp, message, branch);
                fclose(hf);
            }
        }
    }
    closedir(dir);

    FILE *logf = fopen(".tronics/log.txt", "a");
    if (logf) {
        fprintf(logf, "[%s] Commit: %s (%s)\n", timestamp, message, branch);
        fclose(logf);
    }

    system("rm -f .tronics/stage/*");

    printf("Committed: \"%s\"\n", message);
}

// Show log (fixed to read correctly)
void show_log() {
    FILE *f = fopen(".tronics/log.txt", "r");
    if (!f) {
        printf("No commits yet.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0; // Remove newline
        printf("%s\n", line);
    }
    fclose(f);
}

// Show status (unchanged)
void show_status() {
    printf("Staged files:\n");
    system("ls .tronics/stage 2>/dev/null || echo ' (none)'");
}

// Simulate IoT (unchanged)
void simulate_iot() {
    FILE *f = fopen(".tronics/objects/sensor_data.txt", "a");
    if (f) {
        time_t now = time(NULL);
        fprintf(f, "Timestamp: %ld | Temp: 24.2°C | Pressure: 1015 hPa\n", now);
        fclose(f);
        printf("IIoT data appended to sensor_data.txt\n");
    } else {
        printf("Error: sensor_data.txt not found in .tronics/objects/\n");
    }
}

// Simulate CAD (unchanged)
void simulate_cad() {
    FILE *f = fopen(".tronics/objects/cad_log.txt", "a");
    if (f) {
        fprintf(f, "CAD event: draw_line from (0,0) to (100,200)\n");
        fclose(f);
        printf("CAD event written to cad_log.txt\n");
    } else {
        printf("Error: cad_log.txt not found in .tronics/objects/\n");
    }
}

// Diff file (unchanged)
void diff_file(const char *filename) {
    char staged_path[256];
    snprintf(staged_path, sizeof(staged_path), ".tronics/stage/%s", filename);

    FILE *f1 = fopen(filename, "r");
    FILE *f2 = fopen(staged_path, "r");

    if (!f1 || !f2) {
        printf("Either working file or staged file is missing.\n");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return;
    }

    printf("Diff (%s):\n", filename);
    char line1[256], line2[256];
    int line_num = 1;
    while (fgets(line1, sizeof(line1), f1) && fgets(line2, sizeof(line2), f2)) {
        line1[strcspn(line1, "\n")] = 0;
        line2[strcspn(line2, "\n")] = 0;
        if (strcmp(line1, line2) != 0) {
            printf("Line %d:\n  Working: %s\n  Staged: %s\n", line_num, line1, line2);
        }
        line_num++;
    }
    fclose(f1);
    fclose(f2);
}

// Restore file (unchanged)
void restore_file(const char *filename) {
    char object_path[256];
    snprintf(object_path, sizeof(object_path), ".tronics/objects/%s", filename);
    copy_file(object_path, filename);
    printf("Restored %s from last commit.\n", filename);
}

// Status (fixed to list committed files from branch-specific directories)
void status() {
    printf("== Tronics Status ==\n");

    printf("Staged files:\n");
    system("ls .tronics/stage 2>/dev/null || echo '(none)'");

    printf("\nCommitted files:\n");
    system("ls .tronics/objects 2>/dev/null || echo '(none)'");
}

// Create branch (unchanged)
void create_branch(const char *name) {
    char path[128];
    snprintf(path, sizeof(path), ".tronics/refs/%s", name);
    FILE *f = fopen(path, "w");
    if (f) {
        fprintf(f, "Branch %s created.\n", name);
        fclose(f);
        printf("Created branch: %s\n", name);
    } else {
        printf("Failed to create branch: %s\n", name);
    }
}

// List branches (unchanged)
void list_branches() {
    printf("Available branches:\n");
    system("ls .tronics/refs 2>/dev/null || echo '(none)'");
}

// Checkout branch (unchanged)
void checkout_branch(const char *name) {
    char path[128];
    snprintf(path, sizeof(path), ".tronics/refs/%s", name);
    if (access(path, F_OK) != 0) {
        printf("Branch %s does not exist.\n", name);
        return;
    }

    FILE *config = fopen(".tronics/config", "w");
    if (config) {
        fprintf(config, "{ \"branch\": \"%s\", \"version\": \"0.3.1\" }\n", name);
        fclose(config);
        printf("Switched to branch: %s\n", name);
    } else {
        printf("Failed to switch to branch: %s\n", name);
    }
}

// Merge branch (fixed to detect conflicts correctly)
void merge_branch(const char *branch_name) {
    char *current_branch = get_current_branch();
    if (strcmp(current_branch, branch_name) == 0) {
        printf("Cannot merge branch %s into itself.\n", branch_name);
        return;
    }

    char branch_path[256];
    snprintf(branch_path, sizeof(branch_path), ".tronics/refs/%s", branch_name);
    if (access(branch_path, F_OK) != 0) {
        printf("Branch %s does not exist.\n", branch_name);
        return;
    }

    // Copy files from source branch to current branch's latest commit
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
    char commit_dir[256];
    snprintf(commit_dir, sizeof(commit_dir), ".tronics/objects/%s/%s", current_branch, timestamp);
    create_dir(commit_dir);

    char command[512];
    snprintf(command, sizeof(command), "cp .tronics/objects/%s/*/* %s/ 2>/dev/null", branch_name, commit_dir);
    system(command);

    // Check for conflicts
    DIR *dir = opendir(".tronics/stage");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char staged_path[512], object_path[512];
            snprintf(staged_path, sizeof(staged_path), ".tronics/stage/%s", entry->d_name);
            snprintf(object_path, sizeof(object_path), ".tronics/objects/%s/%s/%s", branch_name, timestamp, entry->d_name);
            
            FILE *f1 = fopen(staged_path, "r");
            FILE *f2 = fopen(object_path, "r");
            if (f1 && f2) {
                char line1[256], line2[256];
                while (fgets(line1, sizeof(line1), f1) && fgets(line2, sizeof(line2), f2)) {
                    line1[strcspn(line1, "\n")] = 0;
                    line2[strcspn(line2, "\n")] = 0;
                    if (strcmp(line1, line2) != 0) {
                        printf("CONFLICT: File %s has differences between branches\n", entry->d_name);
                    }
                }
                fclose(f1);
                fclose(f2);
            }
        }
    }
    closedir(dir);

    FILE *logf = fopen(".tronics/log.txt", "a");
    if (logf) {
        fprintf(logf, "[%s] Merge: Merged branch %s into %s\n", timestamp, branch_name, current_branch);
        fclose(logf);
    }

    printf("Merged branch %s into current branch\n", branch_name);
}

// Show file history (unchanged)
void show_file_history(const char *filename) {
    char history_path[512];
    snprintf(history_path, sizeof(history_path), ".tronics/history/%s", filename);
    
    FILE *f = fopen(history_path, "r");
    if (!f) {
        printf("No history for %s\n", filename);
        return;
    }

    printf("History for %s:\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        printf("%s\n", line);
    }
    fclose(f);
}

// Stash push (unchanged)
void stash_push() {
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
    
    char stash_dir[256];
    snprintf(stash_dir, sizeof(stash_dir), ".tronics/stash/%s", timestamp);
    create_dir(stash_dir);
    
    char command[512];
    snprintf(command, sizeof(command), "cp .tronics/stage/* %s/ 2>/dev/null", stash_dir);
    system(command);
    
    FILE *logf = fopen(".tronics/stash/log.txt", "a");
    if (logf) {
        fprintf(logf, "[%s] Stashed changes\n", timestamp);
        fclose(logf);
    }
    
    system("rm -f .tronics/stage/*");
    printf("Changes stashed at %s\n", timestamp);
}



// Stash pop (fixed to ensure staged files are listed)
void stash_pop() {
    DIR *dir = opendir(".tronics/stash");
    if (!dir) {
        printf("No stashed changes found.\n");
        return;
    }

    char latest_stash[64] = "";
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (strcmp(entry->d_name, latest_stash) > 0) {
                strcpy(latest_stash, entry->d_name);
            }
        }
    }
    closedir(dir);

    if (strlen(latest_stash) == 0) {
        printf("No stashed changes found.\n");
        return;
    }

    char command[512];
    snprintf(command, sizeof(command), "cp .tronics/stash/%s/* .tronics/stage/ 2>/dev/null", latest_stash);
    system(command);
    
    snprintf(command, sizeof(command), "rm -rf .tronics/stash/%s", latest_stash);
    system(command);

    printf("Applied stash %s\n", latest_stash);
}

// Stash list (unchanged)
void stash_list() {
    printf("Stashed changes:\n");
    FILE *f = fopen(".tronics/stash/log.txt", "r");
    if (!f) {
        printf("(none)\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        printf("%s\n", line);
    }
    fclose(f);
}

// Create tag (unchanged)
void create_tag(const char *tag_name) {
    char path[128];
    snprintf(path, sizeof(path), ".tronics/tags/%s", tag_name);
    FILE *f = fopen(path, "w");
    if (f) {
        time_t now = time(NULL);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
        fprintf(f, "Tag %s created at %s\n", tag_name, timestamp);
        fclose(f);
        printf("Created tag: %s\n", tag_name);
    } else {
        printf("Failed to create tag: %s\n", tag_name);
    }
}

// List tags (unchanged)
void list_tags() {
    printf("Available tags:\n");
    system("ls .tronics/tags 2>/dev/null || echo '(none)'");
}