#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/capability.h>

#include "sha256.h"
#include "debug.h"

int check_password(char *password) {
    SHA256_CTX ctx;
    BYTE userhash[32];
    char *hash = "\x5e\x88\x48\x98\xda\x28\x04\x71\x51\xd0\xe5\x6f\x8d\xc6\x29\x27\x73\x60\x3d\x0d\x6a\xab\xbd\xd6\x2a\x11\xef\x72\x1d\x15\x42\xd8";
    memcpy(ctx.data, password, strlen(password));

    sha256_init(&ctx);
    sha256_update(&ctx, (const BYTE *)password, strlen(password));
    sha256_final(&ctx, userhash);

    return memcmp(hash, userhash, 32);
    
}


int check_raw_socket(void) {

    int ret = 0xff;
    pid_t current_pid = getpid();
    DBG_PRINT(("[CHECK][i] Current PID: %d\n", current_pid));

    struct __user_cap_header_struct *cap_header_data = malloc(sizeof(struct __user_cap_header_struct));
    cap_header_data->pid = (int)current_pid;
    cap_header_data->version = _LINUX_CAPABILITY_VERSION_3;

    struct __user_cap_data_struct *cap_data = malloc(sizeof(struct __user_cap_data_struct));

    if(capget(cap_header_data, cap_data) < 0) {
        DBG_PRINT(("[CHECK][-] Error getting capabilities"));
        return 1;
    }

    if(cap_data->effective == CAP_TO_MASK(CAP_NET_RAW)) {
        ret = 0;
    }


    free(cap_header_data);
    free(cap_data);
    return ret;

}
