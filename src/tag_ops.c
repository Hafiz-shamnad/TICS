#include "tag_ops.h"
#include "file_ops.h"

void create_tag(const char *tag_name) {
    char path[MAX_PATH];
    size_t len = strlen(tag_name);
    if (len + 14 > MAX_PATH) { // 14 for ".tics/tags/" + null
        printf("Error: Tag name too long: %s\n", tag_name);
        return;
    }
    snprintf(path, sizeof(path), ".tics/tags/%s", tag_name);
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

void list_tags() {
    printf("Available tags:\n");
    system("ls .tics/tags 2>/dev/null || echo '(none)'");
}