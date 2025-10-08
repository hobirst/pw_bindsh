#ifndef DAEMON_H
#define DAEMON_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

pid_t daemon_init(void);

#endif // BACKGROUND_H
