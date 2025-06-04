#include <stdio.h>
#include <string.h>
#include "tronics.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: tronics <command> [args]\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0 && argc == 3) {
        init_repo(argv[2]);
    } 
    else if (strcmp(argv[1], "add") == 0 && argc == 3) {
        add_file(argv[2]);
    } 
    else if (strcmp(argv[1], "commit") == 0 && argc == 4 && strcmp(argv[2], "-m") == 0) {
        commit(argv[3]);
    } 
    else if (strcmp(argv[1], "log") == 0) {
        show_log();
    } 
    else if (strcmp(argv[1], "status") == 0) {
        status();
    } 
    else if (strcmp(argv[1], "diff") == 0 && argc == 3) {
        diff_file(argv[2]);
    } 
    else if (strcmp(argv[1], "restore") == 0 && argc == 3) {
        restore_file(argv[2]);
    } 
    else if (strcmp(argv[1], "branch") == 0 && argc == 3) {
        create_branch(argv[2]);
    } 
    else if (strcmp(argv[1], "list-branches") == 0) {
        list_branches();
    } 
    else if (strcmp(argv[1], "checkout") == 0 && argc == 3) {
        checkout_branch(argv[2]);
    } 
    else if (strcmp(argv[1], "iot") == 0) {
        simulate_iot();
    } 
    else if (strcmp(argv[1], "cad") == 0) {
        simulate_cad();
    } 
    else {
        printf("Unknown command or wrong arguments.\n");
        printf("Available commands:\n");
        printf("  init <repo>\n");
        printf("  add <file>\n");
        printf("  commit -m \"<message>\"\n");
        printf("  log\n");
        printf("  status\n");
        printf("  diff <file>\n");
        printf("  restore <file>\n");
        printf("  branch <name>\n");
        printf("  list-branches\n");
        printf("  checkout <branch>\n");
        printf("  iot\n");
        printf("  cad\n");
    }

    return 0;
}
