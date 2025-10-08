#include <unistd.h>
#include <stdlib.h>

#include "sock.h"
#include "check.h"
#include "daemon.h"
#include "debug.h"

int main(int argc, char **argv) {
    int background = 0;
    int raw_socket = 0;

    int udp_port = 0;
    int tcp_port = 0;

    int c = 0; 
    opterr = 0;

    while((c = getopt(argc, argv, "bu:t:")) != -1) {
        switch(c) {
            case 'b':
                background = 1;
                DBG_PRINT(("[MAIN][i] Daemonize\n"));
                break;
            case 'u':
                udp_port = atoi(optarg);
                DBG_PRINT(("[MAIN][i] UDP Port: %d\n", udp_port));
                break;
            case 't':
                tcp_port = atoi(optarg);
                DBG_PRINT(("[MAIN][i] TCP Port: %d\n", tcp_port));
                break;
        }
    }
    
    if(udp_port == 0 || tcp_port == 0) return 1;

    if(background) {
        pid_t pid = daemon_init();
        if(pid < 0) return pid;
    }

    int euid = geteuid();
    int cap_net_raw_effective = check_raw_socket();
    int sock_fd = 0;

    while(1) {
        if(euid == 0 || cap_net_raw_effective == 0) {
            sock_fd = create_udp_sock(1, udp_port);
            raw_udp(sock_fd, tcp_port, udp_port);
        } else {
            DBG_PRINT(("[MAIN][-] Raw socket not allowed, using default udp\n"));
            sock_fd = create_udp_sock(0, udp_port);
            udp(sock_fd, tcp_port);
        }
        close(sock_fd);
    }

    return 0;



}
