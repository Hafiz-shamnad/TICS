#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

// Function Prototypes
void create_dir(const char *path);
void copy_file(const char *src, const char *dest);
void init_repo(const char *repo_name);
void add_file(const char *filename);
void commit(const char *message);
void show_log();
void show_status();
void simulate_iot();
void simulate_cad();

// Create directory
void create_dir(const char *path) {
    mkdir(path, 0755);
}

// Copy file to staging or object directory
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
        fwrite(buffer, 1, n, fdest);
    }

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

    sprintf(path, "%s/.tronics/log.txt", repo_name);
    FILE *logf = fopen(path, "w");
    fclose(logf);

    sprintf(path, "%s/.tronics/config", repo_name);
    FILE *f = fopen(path, "w");
    if (f) {
        fprintf(f, "{ \"branch\": \"main\", \"version\": \"0.2.0\" }\n");
        fclose(f);
    }

    printf("Initialized empty Tronics repository in %s/.tronics\n", repo_name);
}

// Add file to staging area
void add_file(const char *filename) {
    char dest[512];
    snprintf(dest, sizeof(dest), ".tronics/stage/%s", filename);
    copy_file(filename, dest);
    printf("Staged: %s → .tronics/stage/\n", filename);
}

// Commit staged files
void commit(const char *message) {
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));

    system("mkdir -p .tronics/objects");
    system("cp .tronics/stage/* .tronics/objects/ 2>/dev/null");

    FILE *logf = fopen(".tronics/log.txt", "a");
    if (logf) {
        fprintf(logf, "[%s] Commit: %s\n", timestamp, message);
        fclose(logf);
    }

    system("rm -f .tronics/stage/*");

    printf("Committed: \"%s\"\n", message);
}

// Show commit logs
void show_log() {
    FILE *f = fopen(".tronics/log.txt", "r");
    if (!f) {
        printf("No commits yet.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    fclose(f);
}

// Show status of staged files
void show_status() {
    printf("Staged files:\n");
    system("ls .tronics/stage 2>/dev/null || echo ' (none)'");
}

// Simulate IIoT device data
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

// Simulate CAD drawing event
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
        if (strcmp(line1, line2) != 0) {
            printf("Line %d:\n  Working: %s  Staged: %s", line_num, line1, line2);
        }
        line_num++;
    }
    fclose(f1);
    fclose(f2);
}

void restore_file(const char *filename) {
    char object_path[256];
    snprintf(object_path, sizeof(object_path), ".tronics/objects/%s", filename);
    copy_file(object_path, filename);
    printf("Restored %s from last commit.\n", filename);
}

#include <stdio.h>
#include <stdlib.h>

void status() {
    printf("== Tronics Status ==\n");

    printf("Staged files:\n");
    system("ls .tronics/stage 2>/dev/null || echo '(none)'");

    printf("\nCommitted files:\n");
    system("ls .tronics/objects 2>/dev/null || echo '(none)'");
}

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

void list_branches() {
    printf("Available branches:\n");
    system("ls .tronics/refs 2>/dev/null");
}

void checkout_branch(const char *name) {
    FILE *config = fopen(".tronics/config", "w");
    if (config) {
        fprintf(config, "{ \"branch\": \"%s\", \"version\": \"0.2.0\" }\n", name);
        fclose(config);
        printf("Switched to branch: %s\n", name);
    } else {
        printf("Failed to switch to branch: %s\n", name);
    }
}
