#ifndef TERRARIUM_SYSTEM_SHELL_JOB_CONTROL_H
#define TERRARIUM_SYSTEM_SHELL_JOB_CONTROL_H

int terra_shell_job_background(int pid);
int terra_shell_job_foreground(int pid);
int terra_shell_job_list(void);

#endif
