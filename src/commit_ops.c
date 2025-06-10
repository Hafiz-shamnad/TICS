#include "commit_ops.h"
#include "file_ops.h"
#include "repo_ops.h"

void copy_staged_files(const char *dest_dir) {
    DIR *dir = opendir(".tronics/stage");
    if (!dir) {
        perror("Error opening .tronics/stage");
        return;
    }

    struct dirent *entry;
    char src[MAX_PATH], dest[MAX_PATH];
    int has_files = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            snprintf(src, sizeof(src), ".tronics/stage/%s", entry->d_name);
            snprintf(dest, sizeof(dest), "%s/%s", dest_dir, entry->d_name);
            copy_file(src, dest);
            has_files = 1;
        }
    }
    closedir(dir);
    if (!has_files) {
        printf("Debug: No files found in .tronics/stage\n");
    }
}

void commit(const char *message) {
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
    usleep(100000); // Ensure unique timestamps

    char *branch = get_current_branch();
    char commit_dir[256];
    snprintf(commit_dir, sizeof(commit_dir), ".tronics/objects/%s/%s", branch, timestamp);
    create_dir(commit_dir);

    copy_staged_files(commit_dir);

    // Verify copied files
    DIR *commit_dir_check = opendir(commit_dir);
    if (commit_dir_check) {
        struct dirent *entry;
        int has_files = 0;
        while ((entry = readdir(commit_dir_check)) != NULL) {
            if (entry->d_type == DT_REG) {
                has_files = 1;
            }
        }
        closedir(commit_dir_check);
        if (!has_files) {
            printf("Debug: No files found in commit dir %s\n", commit_dir);
        }
    } else {
        perror("Error opening commit directory for verification");
    }

    DIR *dir = opendir(".tronics/stage");
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG && !is_meta_file(entry->d_name)) {
                char history_path[512];
                snprintf(history_path, sizeof(history_path), ".tronics/history/%s", entry->d_name);
                create_dir(history_path); // Ensure parent directory exists
                FILE *hf = fopen(history_path, "a");
                if (hf) {
                    fprintf(hf, "[%s] %s (%s)\n", timestamp, message, branch);
                    fclose(hf);
                }
            }
        }
        closedir(dir);
    }

    FILE *logf = fopen(".tronics/log.txt", "a");
    if (logf) {
        fprintf(logf, "[%s] Commit: %s (%s)\n", timestamp, message, branch);
        fclose(logf);
    }

    system("rm -f .tronics/stage/*");
    printf("Committed: \"%s\"\n", message);
}

void show_log() {
    FILE *f = fopen(".tronics/log.txt", "r");
    if (!f) {
        printf("No commits yet.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        printf("%s\n", line);
    }
    fclose(f);
}

void show_file_history(const char *filename) {
    char history_path[MAX_PATH];
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

void add_file(const char *filename) {
    char command[MAX_PATH];
    size_t len = strlen(filename);
    if (len + 26 > MAX_PATH) { // 26 for "cp  .tronics/stage/" + null
        printf("Error: File path too long: %s\n", filename);
        return;
    }
    snprintf(command, sizeof(command), "cp %s .tronics/stage/%s", filename, filename);
    if (system(command) == 0) {
        printf("Staged: %s\n", filename);
    } else {
        printf("Failed to stage: %s\n", filename);
    }
}

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

void restore_file(const char *filename) {
    char *branch = get_current_branch();
    char *commit = get_latest_commit(branch);
    char src_path[MAX_PATH];
    size_t len = strlen(filename);
    size_t len_branch = strlen(branch);
    size_t len_commit = strlen(commit);
    if (len_branch + len_commit + len + 24 > MAX_PATH) { // 24 for ".tronics/objects///" + null
        printf("Error: Source path too long for %s/%s/%s\n", branch, commit, filename);
        return;
    }
    snprintf(src_path, sizeof(src_path), ".tronics/objects/%s/%s/%s", branch, commit, filename);
    if (access(src_path, F_OK) == 0) {
        char command[MAX_PATH];
        if (len + 5 > MAX_PATH) { // 5 for "cp  " + null
            printf("Error: Command path too long for %s\n", filename);
            return;
        }
        snprintf(command, sizeof(command), "cp %s %s", src_path, filename);
        system(command);
        printf("Restored: %s\n", filename);
    } else {
        printf("No committed version of %s found.\n", filename);
    }
}