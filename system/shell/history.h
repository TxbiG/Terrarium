#ifndef TERRARIUM_SYSTEM_SHELL_HISTORY_H
#define TERRARIUM_SYSTEM_SHELL_HISTORY_H

int terra_shell_history_add(const char *line);
int terra_shell_history_get(unsigned long index, char *out_line, unsigned long out_size);
int terra_shell_history_save(const char *path);

#endif
