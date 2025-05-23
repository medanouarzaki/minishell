#include "minishell.h"

int minishell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "minishell: « cd » requiert un répertoire\n");
    } else if (chdir(args[1]) != 0) {
        perror("minishell");
    }
    return 1;
}

int minishell_exit(char **args) {
    (void)args;
    return 0;
}

int minishell_help(char **args) {
    (void)args;
    puts("Mini-Shell — commandes intégrées :");
    puts("  cd [dir]");
    puts("  help");
    puts("  exit");
    return 1;
}
