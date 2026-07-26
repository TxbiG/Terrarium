#ifndef TERRARIUM_SYSTEM_SHELL_BUILTINS_H
#define TERRARIUM_SYSTEM_SHELL_BUILTINS_H

typedef int (*terra_shell_builtin_fn_t)(int argc, char **argv);

int terra_shell_register_builtin(const char *name, terra_shell_builtin_fn_t handler);
int terra_shell_builtin_cd(int argc, char **argv);
int terra_shell_builtin_exit(int argc, char **argv);

#endif
