#ifndef TERRARIUM_SHELL_BUILTINS_H
#define TERRARIUM_SHELL_BUILTINS_H

int shell_builtin_cd(int argc, char **argv);
int shell_builtin_exit(int argc, char **argv);
int shell_builtin_pwd(int argc, char **argv);
int shell_builtin_help(int argc, char **argv);
int shell_builtin_source(int argc, char **argv);

#endif
