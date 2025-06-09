#include <stdio.h>
#include <string.h>
#include "tronics.h"

// Updated main function to support Git-like commit syntax
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        printf("Commands: init, add, commit, log, status, diff, restore, branch, checkout, merge, history, stash, pop, list-stash, tag, list-tags, iot, cad\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0 && argc == 3) {
        init_repo(argv[2]);
    } else if (strcmp(argv[1], "add") == 0 && argc == 3) {
        add_file(argv[2]);
    } else if (strcmp(argv[1], "commit") == 0 && argc == 4 && strcmp(argv[2], "-m") == 0) {
        commit(argv[3]);
    } else if (strcmp(argv[1], "log") == 0 && argc == 2) {
        show_log();
    } else if (strcmp(argv[1], "status") == 0 && argc == 2) {
        status();
    } else if (strcmp(argv[1], "diff") == 0 && argc == 3) {
        diff_file(argv[2]);
    } else if (strcmp(argv[1], "restore") == 0 && argc == 3) {
        restore_file(argv[2]);
    } else if (strcmp(argv[1], "branch") == 0 && argc == 3) {
        create_branch(argv[2]);
    } else if (strcmp(argv[1], "list-branches") == 0 && argc == 2) {
        list_branches();
    } else if (strcmp(argv[1], "checkout") == 0 && argc == 3) {
        checkout_branch(argv[2]);
    } else if (strcmp(argv[1], "merge") == 0 && argc == 3) {
        merge_branch(argv[2]);
    } else if (strcmp(argv[1], "history") == 0 && argc == 3) {
        show_file_history(argv[2]);
    } else if (strcmp(argv[1], "stash") == 0 && argc == 2) {
        stash_push();
    } else if (strcmp(argv[1], "pop") == 0 && argc == 2) {
        stash_pop();
    } else if (strcmp(argv[1], "list-stash") == 0 && argc == 2) {
        stash_list();
    } else if (strcmp(argv[1], "tag") == 0 && argc == 3) {
        create_tag(argv[2]);
    } else if (strcmp(argv[1], "list-tags") == 0 && argc == 2) {
        list_tags();
    } else if (strcmp(argv[1], "iot") == 0 && argc == 2) {
        simulate_iot();
    } else if (strcmp(argv[1], "cad") == 0 && argc == 2) {
        simulate_cad();
    } else {
        printf("Unknown command or incorrect usage.\n");
        printf("Commands: init, add, commit, log, status, diff, restore, branch, checkout, merge, history, stash, pop, list-stash, tag, list-tags, iot, cad\n");
        return 1;
    }

    return 0;
}