#include "stash_ops.h"
#include "file_ops.h"

void stash_push() {
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));

    char stash_dir[MAX_PATH];
    size_t len = strlen(timestamp);
    if (len + 14 > MAX_PATH) { // 14 for ".tronics/stash/" + null
        printf("Error: Timestamp too long: %s\n", timestamp);
        return;
    }
    snprintf(stash_dir, sizeof(stash_dir), ".tronics/stash/%s", timestamp);

    char command[MAX_PATH];
    if (strlen(stash_dir) + 34 > MAX_PATH) { // 34 for "cp .tronics/stage/*  2>/dev/null"
        printf("Error: Command path too long for %s\n", stash_dir);
        return;
    }
    snprintf(command, sizeof(command), "cp .tronics/stage/* %s/ 2>/dev/null", stash_dir);
    create_dir(stash_dir);

    system(command);

    FILE *logf = fopen(".tronics/stash/log.txt", "a");
    if (logf) {
        fprintf(logf, "[%s] Stashed changes\n", timestamp);
        fclose(logf);
    }

    system("rm -f .tronics/stage/*");
}

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
                strncpy(latest_stash, entry->d_name, sizeof(latest_stash) - 1);
                latest_stash[sizeof(latest_stash) - 1] = '\0';
            }
        }
    }
    closedir(dir);

    if (strlen(latest_stash) == 0) {
        printf("No stashed changes found.\n");
        return;
    }

    char command[MAX_PATH];
    if (strlen(latest_stash) + 34 > MAX_PATH) { // 34 for "cp .tronics/stash// 2>/dev/null"
        printf("Error: Command path too long for %s\n", latest_stash);
        return;
    }
    snprintf(command, sizeof(command), "cp .tronics/stash/%s/* .tronics/stage/ 2>/dev/null", latest_stash);
    system(command);

    if (strlen(latest_stash) + 20 > MAX_PATH) { // 20 for "rm -rf .tronics/stash/"
        printf("Error: Command path too long for %s\n", latest_stash);
        return;
    }
    snprintf(command, sizeof(command), "rm -rf .tronics/stash/%s", latest_stash);
    system(command);
}

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