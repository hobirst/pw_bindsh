#include "daemon.h"

pid_t daemon_init(void) {
    pid_t pid = fork();
    if(pid < 0) {
        return pid;
    } else if(pid != 0) {
        exit(0);
    }

    if(setsid() < 0) return -1;
    if(chdir("/") < 0) return -2;
    
    umask(0);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);

    return pid;
}
