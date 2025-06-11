#include "branch_ops.h"
#include "file_ops.h"
#include "repo_ops.h"

void create_branch(const char *name)
{
    char path[MAX_PATH];
    size_t len = strlen(name);
    if (len + 14 > MAX_PATH)
    { // 14 for ".tronics/refs/" + null
        printf("Error: Branch name too long: %s\n", name);
        return;
    }
    snprintf(path, sizeof(path), ".tronics/refs/%s", name);
    FILE *f = fopen(path, "w");
    if (f)
    {
        fprintf(f, "Branch %s created.\n", name);
        fclose(f);
        printf("Created branch: %s\n", name);
    }
    else
    {
        printf("Failed to create branch: %s\n", name);
    }
}

void list_branches()
{
    printf("Available branches:\n");
    system("ls .tronics/refs 2>/dev/null || echo '(none)'");
}

void checkout_branch(const char *name)
{
    char path[MAX_PATH];
    size_t len = strlen(name);
    if (len + 14 > MAX_PATH)
    { // 14 for ".tronics/refs/" + null
        printf("Error: Branch name too long: %s\n", name);
        return;
    }
    snprintf(path, sizeof(path), ".tronics/refs/%s", name);
    if (access(path, F_OK) != 0)
    {
        printf("Branch %s does not exist.\n", name);
        return;
    }

    FILE *config = fopen(".tronics/config", "w");
    if (config)
    {
        fprintf(config, "{ \"branch\": \"%s\", \"version\": \"0.3.1\" }\n", name);
        fclose(config);
        printf("Switched to branch: %s\n", name);
    }
    else
    {
        printf("Failed to switch to branch: %s\n", name);
    }
}

char *get_latest_commit(const char *branch)
{
    static char timestamp[64] = "";
    char dir_path[MAX_PATH];
    size_t len = strlen(branch);
    if (len + 16 > MAX_PATH)
    { // 16 for ".tronics/objects/" + null
        printf("Error: Branch name too long: %s\n", branch);
        return timestamp;
    }
    snprintf(dir_path, sizeof(dir_path), ".tronics/objects/%s", branch);
    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        return timestamp;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            if (strlen(entry->d_name) > 63)
            { // Limit timestamp length
                printf("Error: Timestamp too long: %s\n", entry->d_name);
                continue;
            }
            if (strcmp(entry->d_name, timestamp) > 0)
            {
                strncpy(timestamp, entry->d_name, sizeof(timestamp));
                timestamp[sizeof(timestamp) - 1] = '\0';
            }
        }
    }
    closedir(dir);
    return timestamp;
}
void merge_branch(const char *branch_name)
{
    char *current_branch = get_current_branch();
    if (!current_branch)
    {
        printf("Error: Could not determine current branch\n");
        return;
    }
    if (strcmp(current_branch, branch_name) == 0)
    {
        printf("Cannot merge branch %s into itself.\n", branch_name);
        return;
    }

    char branch_path[1024];
    snprintf(branch_path, sizeof(branch_path), ".tronics/refs/%s", branch_name);
    if (access(branch_path, F_OK) != 0)
    {
        printf("Branch %s does not exist.\n", branch_name);
        return;
    }

    char *source_timestamp = get_latest_commit(branch_name);
    if (!source_timestamp || strlen(source_timestamp) == 0)
    {
        printf("Error: No commits found in branch %s\n", branch_name);
        return;
    }

    char *current_timestamp = get_latest_commit(current_branch);
    if (!current_timestamp || strlen(current_timestamp) == 0)
    {
        printf("Warning: No prior commits in %s, proceeding with merge\n", current_branch);
    }

    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", localtime(&now));
    char commit_dir[1024];
    snprintf(commit_dir, sizeof(commit_dir), ".tronics/objects/%s/%s", current_branch, timestamp);
    create_dir(commit_dir);
    if (access(commit_dir, F_OK) != 0)
    {
        printf("Error: Failed to create commit directory %s\n", commit_dir);
        return;
    }

    char source_commit_dir[1024];
    snprintf(source_commit_dir, sizeof(source_commit_dir), ".tronics/objects/%s/%s", branch_name, source_timestamp);
    DIR *source_dir = opendir(source_commit_dir);
    if (!source_dir)
    {
        printf("Error: Could not open source commit directory %s\n", source_commit_dir);
        return;
    }

    int has_conflict = 0;
    struct dirent *entry;

    while ((entry = readdir(source_dir)) != NULL)
    {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".stl") && !strstr(entry->d_name, ".meta"))
        {
            char base_name[1024];
            strncpy(base_name, entry->d_name, sizeof(base_name));
            base_name[strlen(base_name)] = '\0';

            char source_stl[2048], source_meta[2048];
            snprintf(source_stl, sizeof(source_stl), "%s/%s", source_commit_dir, base_name);
            snprintf(source_meta, sizeof(source_meta), "%s/%s.meta", source_commit_dir, base_name);

            char dest_stl[2048], dest_meta[2048];
            snprintf(dest_stl, sizeof(dest_stl), "%s/%s", commit_dir, base_name);
            snprintf(dest_meta, sizeof(dest_meta), "%s/%s.meta", commit_dir, base_name);

            char current_meta[2048];
            snprintf(current_meta, sizeof(current_meta), ".tronics/objects/%s/%s/%s.meta", current_branch, current_timestamp, base_name);

            if (access(source_meta, F_OK) != 0)
            {
                printf("Warning: Source metadata missing for %s\n", base_name);
                continue;
            }

            if (strlen(current_timestamp) > 0 && access(current_meta, F_OK) == 0)
            {
                FILE *f1 = fopen(current_meta, "rb");
                FILE *f2 = fopen(source_meta, "rb");
                if (!f1 || !f2)
                {
                    if (f1)
                        fclose(f1);
                    if (f2)
                        fclose(f2);
                    printf("CONFLICT: Could not open metadata files for %s\n", base_name);
                    has_conflict = 1;
                    continue;
                }

                MD5_CTX ctx1, ctx2;
                unsigned char hash1[MD5_DIGEST_LENGTH], hash2[MD5_DIGEST_LENGTH];
                unsigned char buffer[4096];
                size_t bytes;

                MD5_Init(&ctx1);
                while ((bytes = fread(buffer, 1, sizeof(buffer), f1)) > 0)
                    MD5_Update(&ctx1, buffer, bytes);
                MD5_Final(hash1, &ctx1);
                fclose(f1);

                MD5_Init(&ctx2);
                while ((bytes = fread(buffer, 1, sizeof(buffer), f2)) > 0)
                    MD5_Update(&ctx2, buffer, bytes);
                MD5_Final(hash2, &ctx2);
                fclose(f2);

                if (memcmp(hash1, hash2, MD5_DIGEST_LENGTH) != 0)
                {
                    printf("CONFLICT: Metadata differences for %s\n", base_name);
                    has_conflict = 1;
                    continue;
                }
            }

            copy_file(source_stl, dest_stl);
            copy_file(source_meta, dest_meta);

            if (access(dest_stl, F_OK) != 0 || access(dest_meta, F_OK) != 0)
                printf("Warning: Failed to copy %s or its metadata to commit directory\n", base_name);
        }
    }
    closedir(source_dir);

    if (strlen(current_timestamp) > 0)
    {
        char current_commit_dir[1024];
        snprintf(current_commit_dir, sizeof(current_commit_dir), ".tronics/objects/%s/%s", current_branch, current_timestamp);
        DIR *current_dir = opendir(current_commit_dir);
        if (current_dir)
        {
            while ((entry = readdir(current_dir)) != NULL)
            {
                if (entry->d_type == DT_REG && strstr(entry->d_name, ".stl") && !strstr(entry->d_name, ".meta"))
                {
                    char base_name[1024];
                    strncpy(base_name, entry->d_name, sizeof(base_name));
                    base_name[strlen(base_name)] = '\0';

                    char source_stl[2048];
                    snprintf(source_stl, sizeof(source_stl), "%s/%s", source_commit_dir, base_name);
                    if (access(source_stl, F_OK) != 0)
                    {
                        printf("CONFLICT: File %s exists in %s but not in %s\n", base_name, current_branch, branch_name);
                        has_conflict = 1;
                        continue;
                    }

                    char current_stl[2048], current_meta[2048];
                    snprintf(current_stl, sizeof(current_stl), "%s/%s", current_commit_dir, base_name);
                    snprintf(current_meta, sizeof(current_meta), "%s/%s.meta", current_commit_dir, base_name);

                    char dest_stl[2048], dest_meta[2048];
                    snprintf(dest_stl, sizeof(dest_stl), "%s/%s", commit_dir, base_name);
                    snprintf(dest_meta, sizeof(dest_meta), "%s/%s.meta", commit_dir, base_name);

                    if (access(dest_stl, F_OK) != 0)
                    {
                        copy_file(current_stl, dest_stl);
                        copy_file(current_meta, dest_meta);
                        if (access(dest_stl, F_OK) != 0 || access(dest_meta, F_OK) != 0)
                            printf("Warning: Failed to copy %s or its metadata from current branch\n", base_name);
                    }
                }
            }
            closedir(current_dir);
        }
    }

    if (has_conflict)
    {
        printf("Error: Merge aborted due to conflicts\n");
        char command[2048];
        snprintf(command, sizeof(command), "rm -rf %s", commit_dir);
        system(command);
        return;
    }

    FILE *logf = fopen(".tronics/log.txt", "a");
    if (logf)
    {
        fprintf(logf, "[%s] Merge: Merged branch %s into %s\n", timestamp, branch_name, current_branch);
        fclose(logf);
    }

    printf("Merged branch %s into %s (commit %s)\n", branch_name, current_branch, timestamp);
}
