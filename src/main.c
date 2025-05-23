#include "minishell.h"
#include <fcntl.h>

int execute(char **args) {
    if (args[0] == NULL)
        return 1;

    const char *builtins[] = {"cd", "help", "exit"};
    int (*builtin_funcs[])(char **) = { minishell_cd, minishell_help, minishell_exit };

    for (size_t i = 0; i < sizeof(builtins) / sizeof(char *); ++i)
        if (strcmp(args[0], builtins[i]) == 0)
            return (*builtin_funcs[i])(args);

    return launch(args);
}

int launch(char **args) {
    int in_fd = -1, out_fd = -1;
    int i = 0;

    // Gère les redirections
    while (args[i]) {
        if (strcmp(args[i], ">") == 0 && args[i + 1]) {
            out_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0 && args[i + 1]) {
            out_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            args[i] = NULL;
        } else if (strcmp(args[i], "<") == 0 && args[i + 1]) {
            in_fd = open(args[i + 1], O_RDONLY);
            args[i] = NULL;
        }
        i++;
    }

    pid_t pid = fork();
    if (pid == 0) {
        if (in_fd != -1)
            dup2(in_fd, STDIN_FILENO);
        if (out_fd != -1)
            dup2(out_fd, STDOUT_FILENO);

        execvp(args[0], args);
        perror("minishell");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("minishell");
    } else {
        int status;
        waitpid(pid, &status, 0);
    }

    if (in_fd != -1) close(in_fd);
    if (out_fd != -1) close(out_fd);
    return 1;
}


char *expand_var(char *token) {
    if (token[0] != '$')
        return token;

    char *value = getenv(token + 1); // retire le $
    return value ? value : "";
}

char **split_line(char *line) {
    size_t bufsize = 64, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "minishell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n");
    while (token) {
        token = expand_var(token); // Ajout ici 🔥
        tokens[pos++] = token;
        if (pos >= bufsize) {
            bufsize *= 2;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "minishell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[pos] = NULL;
    return tokens;
}

void minishell_loop(void) {
    char *line = NULL;
    size_t len = 0;
    char **args;
    int status = 1;

    do {
        printf("mini$ ");
        if (getline(&line, &len, stdin) == -1) {
            if (feof(stdin)) { putchar('\n'); break; }
            perror("getline");
            continue;
        }
        args = split_line(line);
        status = execute(args);
        free(args);
    } while (status);

    free(line);
}

int main(void) {
    minishell_loop();
    return EXIT_SUCCESS;
}

void sigint_handler(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\nmini$ ", 8);
}
