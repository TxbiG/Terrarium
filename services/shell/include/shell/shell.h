#ifndef TERRARIUM_SHELL_H
#define TERRARIUM_SHELL_H

void shell_run(void);
int shell_run_line(char *line);
int shell_run_script(const char *path);

#endif
