#ifndef SOCK_H
#define SOCK_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/udp.h>
#include <linux/ip.h>

#include "sha256.h"

int create_udp_sock(int raw, int udp_port);
int raw_udp(int sock_fd, int tcp_port, int udp_port);
int udp(int sock_fd, int tcp_port);
int tcp(int port);

#endif // SOCK_H
