#ifndef COMMON_H
#define COMMON_H

#define SERVER_PORT 8989
#define MAX_CLIENT 10
#define MAX_MSG_LEN 2048

void onAccept(int srvFd, short event, void *arg);

void onRead(int cliFd, short event, void *arg);

int make_socket_nonblocking(int fd);
#endif //COMMON_H
