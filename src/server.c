#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <event.h>

#include "cLog.h"
#include "common.h"

struct event_base *gBase = NULL;
struct event evListen;
struct event evRead;
int main(int argc, char **argv)
{
	int srvFd = -1;

	struct sockaddr_in sSrvAddr;
	//struct event evListen;

	const char *libeventVersion = event_get_version();
	if (libeventVersion)
	{
		LOG(DEBUG, "server base on libevent: %s\n", libeventVersion);
	}
	else
	{
		LOG(DEBUG, "get libevent version failed\n");
	}

	LOG(DEBUG, "STEP 1: event_base_new\n");
	if (NULL == (gBase = event_base_new()))
	{
		LOG(DEBUG, "Couldn't get an event_base! exiting\n");
		return -1;
	}

	LOG(DEBUG, "STEP 2: event_set\n");
	memset(&sSrvAddr, 0, sizeof(sSrvAddr));
	sSrvAddr.sin_family = AF_INET;
	sSrvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sSrvAddr.sin_port = htons(SERVER_PORT);

	if ((srvFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		LOG(DEBUG, "Create socket failed, errno(%d)\n", errno);
		return -1;
	}

	if (make_socket_nonblocking(srvFd) != 0)
	{
		LOG(DEBUG, "make socket(%d) nonblocking failed\n", srvFd);
		return -1;
	}

	if (bind(srvFd, (struct sockaddr*)&sSrvAddr, sizeof(sSrvAddr)) == -1)
	{
		LOG(DEBUG, "bind failed, errno(%d)\n", errno);
		return -1;
	}
	if (listen(srvFd, MAX_CLIENT) == -1)
	{
		LOG(DEBUG, "listen failed, errno(%d)\n", errno);
		return -1;
	}

	event_set(&evListen, srvFd, EV_READ | EV_PERSIST, onAccept, NULL);


	LOG(DEBUG, "STEP 3: event_base_set\n");
	event_base_set(gBase, &evListen);

	LOG(DEBUG, "STEP 4: event_add\n");
	event_add(&evListen, NULL);

	LOG(DEBUG, "STEP 5: event_base_dispatch\n");
	event_base_dispatch(gBase);

	event_base_free(gBase);

	return 0;
}

void onAccept(int srvFd, short event, void *arg)
{
	int cliFd = -1;
	struct sockaddr_in cliAddr;
	//struct event *evRead = (struct event*)malloc(sizeof(struct event));
	// use a local event would lead to a segment fault before the onRead is called
	//struct event evRead;

	LOG(DEBUG, "onAccept\n");

	socklen_t sinSize = sizeof(cliAddr);
	if ((cliFd = accept(srvFd, (struct sockaddr*)&cliAddr, &sinSize)) < 0)
	{
		LOG(DEBUG, "accept failed\n");
		return;
	}
	else
	{
		LOG(DEBUG, "client(%d) connected\n", cliFd);
	}

	/*event_set(evRead, cliFd, EV_READ|EV_PERSIST, onRead, evRead);
	event_base_set(gBase, evRead);
	event_add(evRead, NULL);*/
	event_set(&evRead, cliFd, EV_READ|EV_PERSIST, onRead, &evRead);
	event_base_set(gBase, &evRead);
	event_add(&evRead, NULL);
	//free(evRead);
	return;
}

void onRead(int cliFd, short event, void *arg)
{
	LOG(DEBUG, "onRead\n");

	char buf[MAX_MSG_LEN];
	int len;
	len = recv(cliFd, buf, MAX_MSG_LEN, 0);
	if (len <= 0)
	{
		LOG(DEBUG, "client(%d) already left\n", cliFd);
		if (arg)
			event_del((struct event*)arg);

		//close(cliFd);
	}
	else
	{
		buf[len] = '\0';
		LOG(DEBUG, "client msg: %s\n", buf);
	}
	return;
}
