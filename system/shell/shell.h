#ifndef TERRARIUM_SYSTEM_SHELL_SHELL_H
#define TERRARIUM_SYSTEM_SHELL_SHELL_H

int terra_shell_run(void);
int terra_shell_execute_line(const char *line);
int terra_shell_run_script(const char *path);

#endif
