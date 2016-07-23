#include <stdio.h>
#include "../include/cLog.h"

int cLog(int lev, char *format, ...)
{
	char logMsg[MAX_LOG_MSG_LEN];
	/*va_list args;
	va_start(args, format);
	vsprintf(logMsg, format, args);
	va_end(args);
	printf("%s", logMsg);*/
	return 0;
}
