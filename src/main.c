#include "repo_ops.h"
#include "commit_ops.h"
#include "branch_ops.h"
#include "stash_ops.h"
#include "cad_ops.h"
#include "tag_ops.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        printf("Commands: init, add, commit, status, log, branch, checkout, merge, stash, pop, list, tag, list-tags, iot, cad, diff, restore, add-cad, diff-cad, history\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        if (argc != 3) {
            printf("Usage: %s init <repo_name>\n", argv[0]);
            return 1;
        }
        init_repo(argv[2]);
    }
    else if (strcmp(argv[1], "add") == 0) {
        if (argc != 3) {
            printf("Usage: %s add <filename>\n", argv[0]);
            return 1;
        }
        add_file(argv[2]);
    }
    else if (strcmp(argv[1], "commit") == 0 && argc == 4 && strcmp(argv[2], "-m") == 0) {
        if (argc != 4) {
            printf("Usage: %s commit -m <message>\n", argv[0]);
            return 1;
        }
        commit(argv[3]);
    }
    else if (strcmp(argv[1], "list-branches") == 0) {
        list_branches();
    }
    else if (strcmp(argv[1], "status") == 0) {
        status();
    }
    else if (strcmp(argv[1], "log") == 0) {
        show_log();
    }
    else if (strcmp(argv[1], "branch") == 0) {
        if (argc != 3) {
            printf("Usage: %s branch <name>\n", argv[0]);
            return 1;
        }
        create_branch(argv[2]);
    }
    else if (strcmp(argv[1], "checkout") == 0) {
        if (argc != 3) {
            printf("Usage: %s checkout <name>\n", argv[0]);
            return 1;
        }
        checkout_branch(argv[2]);
    }
    else if (strcmp(argv[1], "merge") == 0) {
        if (argc != 3) {
            printf("Usage: %s merge <branch_name>\n", argv[0]);
            return 1;
        }
        merge_branch(argv[2]);
    }
    else if (strcmp(argv[1], "stash") == 0) {
        stash_push();
    }
    else if (strcmp(argv[1], "pop") == 0) {
        stash_pop();
    }
    else if (strcmp(argv[1], "list") == 0) {
        stash_list();
    }
    else if (strcmp(argv[1], "tag") == 0) {
        if (argc != 3) {
            printf("Usage: %s tag <tag_name>\n", argv[0]);
            return 1;
        }
        create_tag(argv[2]);
    }
    else if (strcmp(argv[1], "list-tags") == 0) {
        list_tags();
    }
    else if (strcmp(argv[1], "iot") == 0) {
        simulate_iot();
    }
    else if (strcmp(argv[1], "cad") == 0) {
        simulate_cad();
    }
    else if (strcmp(argv[1], "diff") == 0) {
        if (argc != 3) {
            printf("Usage: %s diff <filename>\n", argv[0]);
            return 1;
        }
        diff_file(argv[2]);
    }
    else if (strcmp(argv[1], "restore") == 0) {
        if (argc != 3) {
            printf("Usage: %s restore <filename>\n", argv[0]);
            return 1;
        }
        restore_file(argv[2]);
    }
    else if (strcmp(argv[1], "add-cad") == 0) {
        if (argc != 3) {
            printf("Usage: %s add-cad <filename>\n", argv[0]);
            return 1;
        }
        add_cad(argv[2]);
    }
    else if (strcmp(argv[1], "diff-cad") == 0) {
        if (argc != 3) {
            printf("Usage: %s diff-cad <filename>\n", argv[0]);
            return 1;
        }
        diff_cad(argv[2]);
    }
    else if (strcmp(argv[1], "history") == 0) {
        if (argc != 3) {
            printf("Usage: %s history <filename>\n", argv[0]);
            return 1;
        }
        show_file_history(argv[2]);
    }
    else {
        printf("Unknown command: %s\n", argv[1]);
        return 1;
    }

    return 0;
}