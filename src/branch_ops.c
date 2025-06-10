#include "branch_ops.h"
#include "file_ops.h"
#include "repo_ops.h"

void create_branch(const char *name) {
    char path[MAX_PATH];
    size_t len = strlen(name);
    if (len + 14 > MAX_PATH) { // 14 for ".tronics/refs/" + null
        printf("Error: Branch name too long: %s\n", name);
        return;
    }
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

void list_branches() {
    printf("Available branches:\n");
    system("ls .tronics/refs 2>/dev/null || echo '(none)'");
}

void checkout_branch(const char *name) {
    char path[MAX_PATH];
    size_t len = strlen(name);
    if (len + 14 > MAX_PATH) { // 14 for ".tronics/refs/" + null
        printf("Error: Branch name too long: %s\n", name);
        return;
    }
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

char *get_latest_commit(const char *branch) {
    static char timestamp[64] = "";
    char dir_path[MAX_PATH];
    size_t len = strlen(branch);
    if (len + 16 > MAX_PATH) { // 16 for ".tronics/objects/" + null
        printf("Error: Branch name too long: %s\n", branch);
        return timestamp;
    }
    snprintf(dir_path, sizeof(dir_path), ".tronics/objects/%s", branch);
    DIR *dir = opendir(dir_path);
    if (!dir) {
        return timestamp;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            if (strlen(entry->d_name) > 63) { // Limit timestamp length
                printf("Error: Timestamp too long: %s\n", entry->d_name);
                continue;
            }
            if (strcmp(entry->d_name, timestamp) > 0) {
                strncpy(timestamp, entry->d_name, sizeof(timestamp));
                timestamp[sizeof(timestamp) - 1] = '\0';
            }
        }
    }
    closedir(dir);
    return timestamp;
}

void merge_branch(const char *branch_name) {
    char *current_branch = get_current_branch();
    if (strcmp(current_branch, branch_name) == 0) {
        printf("Cannot merge branch %s into itself.\n", branch_name);
        return;
    }

    char branch_path[MAX_PATH];
    size_t len = strlen(branch_name);
    if (len + 14 > MAX_PATH) { // 14 for ".tronics/refs/" + null
        printf("Error: Branch name too long: %s\n", branch_name);
        return;
    }
    snprintf(branch_path, sizeof(branch_path), ".tronics/refs/%s", branch_name);
    if (access(branch_path, F_OK) != 0) {
        printf("Branch %s does not exist.\n", branch_name);
        return;
    }

    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
    char commit_dir[MAX_PATH];
    len = strlen(current_branch) + strlen(timestamp);
    if (len + 18 > MAX_PATH) { // 18 for ".tronics/objects//" + null
        printf("Error: Commit directory path too long\n");
        return;
    }
    snprintf(commit_dir, sizeof(commit_dir), ".tronics/objects/%s/%s", current_branch, timestamp);
    create_dir(commit_dir);

    char *source_timestamp = get_latest_commit(branch_name);
    if (strlen(source_timestamp) == 0) {
        printf("Error: No commits found in branch %s\n", branch_name);
        return;
    }

    char command[MAX_PATH];
    len = strlen(branch_name) + strlen(source_timestamp) + strlen(commit_dir);
    if (len + 38 > MAX_PATH) { // 38 for "cp .tronics/objects///*  2>/dev/null"
        printf("Error: Command path too long for %s/%s/* to %s\n", branch_name, source_timestamp, commit_dir);
        return;
    }
    snprintf(command, sizeof(command), "cp .tronics/objects/%s/%s/* %s/ 2>/dev/null", branch_name, source_timestamp, commit_dir);
    system(command);

    DIR *dir = opendir(commit_dir);
    if (!dir) {
        printf("Error: Could not open commit directory %s\n", commit_dir);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".stl")) {
            char current_path[MAX_PATH], source_path[MAX_PATH];
            char *current_timestamp = get_latest_commit(current_branch);
            if (strlen(current_timestamp) == 0) {
                printf("Warning: No prior commits in %s, skipping conflict check for %s\n", current_branch, entry->d_name);
                continue;
            }
            size_t len1 = strlen(current_branch) + strlen(current_timestamp) + strlen(entry->d_name);
            size_t len2 = strlen(branch_name) + strlen(source_timestamp) + strlen(entry->d_name);
            if (len1 + 24 > MAX_PATH || len2 + 24 > MAX_PATH) { // 24 for ".tronics/objects//.meta"
                printf("Error: Metadata path too long for %s\n", entry->d_name);
                continue;
            }
            snprintf(current_path, sizeof(current_path), ".tronics/objects/%s/%s/%s.meta", current_branch, current_timestamp, entry->d_name);
            snprintf(source_path, sizeof(source_path), ".tronics/objects/%s/%s/%s.meta", branch_name, source_timestamp, entry->d_name);

            if (access(current_path, F_OK) == 0 && access(source_path, F_OK) == 0) {
                FILE *f1 = fopen(current_path, "r");
                FILE *f2 = fopen(source_path, "r");
                if (f1 && f2) {
                    char line1[256], line2[256];
                    int has_conflict = 0;
                    while (fgets(line1, sizeof(line1), f1) && fgets(line2, sizeof(line2), f2)) {
                        line1[strcspn(line1, "\n")] = 0;
                        line2[strcspn(line2, "\n")] = 0;
                        if (strcmp(line1, line2) != 0) {
                            has_conflict = 1;
                            break;
                        }
                    }
                    if (has_conflict) {
                        printf("CONFLICT: CAD file %s has metadata differences\n", entry->d_name);
                    }
                    fclose(f1);
                    fclose(f2);
                } else {
                    if (f1) fclose(f1);
                    if (f2) fclose(f2);
                    printf("Warning: Could not open metadata files for %s\n", entry->d_name);
                }
            } else {
                printf("Warning: Metadata file missing for %s in one or both branches\n", entry->d_name);
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