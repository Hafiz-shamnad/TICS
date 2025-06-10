#include "cad_ops.h"
#include "file_ops.h"
#include "commit_ops.h"
#include "repo_ops.h"

void simulate_iot() {
    char *branch = get_current_branch();
    char timestamp[64];
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
    char path[MAX_PATH];
    snprintf(path, sizeof(path), ".tronics/objects/%s/%s", branch, timestamp);
    create_dir(path);
    snprintf(path, sizeof(path), ".tronics/objects/%s/%s/sensor_data.txt", branch, timestamp);
    FILE *f = fopen(path, "a");
    if (f) {
        fprintf(f, "Timestamp: %ld | Temp: 24.2°C | Pressure: 1015 hPa\n", now);
        fclose(f);
        printf("IIoT data appended to sensor_data.txt\n");
    } else {
        perror("Error: Failed to write sensor_data.txt");
    }
}

void simulate_cad() {
    char *branch = get_current_branch();
    char timestamp[64];
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
    char path[MAX_PATH];
    snprintf(path, sizeof(path), ".tronics/objects/%s/%s", branch, timestamp);
    create_dir(path);
    snprintf(path, sizeof(path), ".tronics/objects/%s/%s/cad_log.txt", branch, timestamp);
    FILE *f = fopen(path, "a");
    if (f) {
        fprintf(f, "CAD event: draw_line from (0,0) to (100,200)\n");
        fclose(f);
        printf("CAD event written to cad_log.txt\n");
    } else {
        perror("Error: Failed to write cad_log.txt");
    }
}

int parse_stl_vertices(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Error opening STL file");
        return -1;
    }

    int vertex_count = 0;
    char line[256];
    int is_ascii = 0;

    if (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "solid ", 6) == 0) {
            is_ascii = 1;
        }
    }
    rewind(f);

    if (is_ascii) {
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, "vertex ")) {
                vertex_count++;
            }
        }
    } else {
        fseek(f, 84, SEEK_SET);
        u_int32_t num_triangles;
        if (fread(&num_triangles, sizeof(u_int32_t), 1, f) == 1) {
            vertex_count = num_triangles * 3;
        }
    }

    fclose(f);
    return vertex_count > 0 ? vertex_count : -1;
}

void add_cad(const char *filename) {
    char dest[512];
    snprintf(dest, sizeof(dest), ".tronics/stage/%s", filename);
    copy_file(filename, dest);

    char meta_path[512];
    snprintf(meta_path, sizeof(meta_path), ".tronics/stage/%s.meta", filename);
    FILE *meta = fopen(meta_path, "w");
    if (meta) {
        struct stat st;
        if (stat(filename, &st) == 0) {
            int vertices = parse_stl_vertices(filename);
            fprintf(meta, "size:%ld\nvertices:%d\n", st.st_size, vertices >= 0 ? vertices : 0);
            fclose(meta);
            printf("Staged CAD: %s → .tronics/stage/\n", filename);
        } else {
            perror("Error getting file stats");
            fclose(meta);
        }
    } else {
        perror("Error creating metadata file");
    }
}

void diff_cad(const char *filename) {
    if (strstr(filename, ".stl") == NULL) {
        printf("Error: File %s is not an STL file.\n", filename);
        return;
    }

    char meta_path[MAX_PATH];
    snprintf(meta_path, sizeof(meta_path), ".tronics/stage/%s.meta", filename);

    if (access(meta_path, F_OK) == 0) {
        printf("CAD Metadata Diff (%s):\n", filename);
        FILE *f = fopen(meta_path, "r");
        if (f) {
            char line[256];
            while (fgets(line, sizeof(line), f)) {
                printf("%s", line);
            }
            fclose(f);
        } else {
            printf("Error: Could not open metadata file %s.\n", meta_path);
        }
    } else {
        printf("Error: Metadata file %s not found.\n", meta_path);
    }
}