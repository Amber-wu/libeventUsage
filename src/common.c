#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include "common.h"
#include "cLog.h"

int make_socket_nonblocking(int fd)
{
	int flags;
	if ((flags = fcntl(fd, F_GETFL, NULL)) < 0)
	{
		LOG(DEBUG, "fcntl(%d, F_GETFL) failed\n", fd);
		return -1;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		LOG(DEBUG, "fcntl(%d, F_SETFL) failed\n", fd);
		return -1;
	}
	return 0;
}
