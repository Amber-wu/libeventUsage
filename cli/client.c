#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"
#include "cLog.h"

#define CLIENT_MSG "hello from client"

int main(int argc, char **argv)
{
	int cliFd = -1;

	if ((cliFd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{
		LOG(DEBUG, "client create socket failed\n");
		return -1;
	}

	struct sockaddr_in srvAddr;
	memset(&srvAddr, 0, sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srvAddr.sin_port = htons(SERVER_PORT);

	if (-1 == connect(cliFd, (struct sockaddr*)&srvAddr, sizeof(srvAddr)))
	{
		LOG(DEBUG, "client connect failed, errno(%d)\n", errno);
		return -1;
	}

	printf("send(%d)\n", strlen(CLIENT_MSG));
	write(cliFd, CLIENT_MSG, strlen(CLIENT_MSG));

	close(cliFd);
	return 0;
}
