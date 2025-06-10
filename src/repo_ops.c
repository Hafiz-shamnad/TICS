#include "repo_ops.h"
#include "file_ops.h"

void init_repo(const char *repo_name) {
    char path[MAX_PATH];

    snprintf(path, sizeof(path), "%s", repo_name);
    create_dir(path);

    snprintf(path, sizeof(path), "%s/.tronics", repo_name);
    create_dir(path);

    snprintf(path, sizeof(path), "%s/.tronics/objects", repo_name);
    create_dir(path);

    snprintf(path, sizeof(path), "%s/.tronics/refs", repo_name);
    create_dir(path);

    snprintf(path, sizeof(path), "%s/.tronics/stash", repo_name);
    create_dir(path);

    snprintf(path, sizeof(path), "%s/.tronics/stage", repo_name);
    create_dir(path);

    snprintf(path, sizeof(path), "%s/.tronics/tags", repo_name);
    create_dir(path);

    snprintf(path, sizeof(path), "%s/.tronics/history", repo_name);
    create_dir(path);

    snprintf(path, sizeof(path), "%s/.tronics/log.txt", repo_name);
    FILE *logf = fopen(path, "w");
    fclose(logf);

    snprintf(path, sizeof(path), "%s/.tronics/stash/log.txt", repo_name);
    logf = fopen(path, "w");
    fclose(logf);

    snprintf(path, sizeof(path), "%s/.tronics/config", repo_name);
    FILE *f = fopen(path, "w");
    if (f) {
        fprintf(f, "{ \"branch\": \"main\", \"version\": \"0.3.1\" }\n");
        fclose(f);
    }

    snprintf(path, sizeof(path), "%s/.tronics/refs/main", repo_name);
    f = fopen(path, "w");
    if (f) {
        fprintf(f, "Branch main created.\n");
        fclose(f);
    }

    printf("Initialized empty Tronics repository in %s/.tronics\n", repo_name);
}

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

void status(void) {
    char *current_branch = get_current_branch();
    printf("== Tronics Status ==\n");

    printf("Staged files:\n");
    DIR *stage_dir = opendir(".tronics/stage");
    int has_staged = 0;
    if (stage_dir) {
        struct dirent *entry;
        while ((entry = readdir(stage_dir)) != NULL) {
            if (entry->d_type == DT_REG && !is_meta_file(entry->d_name)) {
                printf("  %s\n", entry->d_name);
                has_staged = 1;
            }
        }
        closedir(stage_dir);
    } else {
        perror("Debug: Failed to open .tronics/stage");
    }
    if (!has_staged) {
        printf("  (none)\n");
    }

    printf("Committed files:\n");
    char commit_path[MAX_PATH];
    snprintf(commit_path, sizeof(commit_path), ".tronics/objects/%s", current_branch);
    DIR *branch_dir = opendir(commit_path);
    int has_committed = 0;
    if (branch_dir) {
        struct dirent *commit_entry;
        while ((commit_entry = readdir(branch_dir)) != NULL) {
            if (commit_entry->d_type == DT_DIR && strcmp(commit_entry->d_name, ".") != 0 && strcmp(commit_entry->d_name, "..") != 0) {
                char timestamp_path[MAX_PATH];
                size_t len1 = strlen(commit_path);
                size_t len2 = strlen(commit_entry->d_name);
                if (len2 > 255) { // Limit d_name length
                    printf("Error: Directory name too long: %s\n", commit_entry->d_name);
                    continue;
                }
                if (len1 + len2 + 2 > MAX_PATH) { // +2 for '/' and null terminator
                    printf("Error: Path too long for %s/%s\n", commit_path, commit_entry->d_name);
                    continue;
                }
                snprintf(timestamp_path, sizeof(timestamp_path), "%s/%s", commit_path, commit_entry->d_name);
                DIR *timestamp_dir = opendir(timestamp_path);
                if (timestamp_dir) {
                    struct dirent *file_entry;
                    while ((file_entry = readdir(timestamp_dir)) != NULL) {
                        if (file_entry->d_type == DT_REG && !is_meta_file(file_entry->d_name)) {
                            printf("  %s\n", file_entry->d_name);
                            has_committed = 1;
                        }
                    }
                    closedir(timestamp_dir);
                } else {
                    perror("Debug: Failed to open timestamp dir");
                    printf("Debug: Path was %s\n", timestamp_path);
                }
            }
        }
        closedir(branch_dir);
    } else {
        perror("Debug: Failed to open commit path");
        printf("Debug: Path was %s\n", commit_path);
    }
    if (!has_committed) {
        printf("  (none)\n");
    }

    printf("Current branch:\n  %s\n", current_branch);
}