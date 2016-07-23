#ifndef CLOG_H
#define CLOG_H

#include <stdio.h>
#include <stdarg.h>

#define MAX_LOG_MSG_LEN 1024

enum logLevel {ERR = 0, WARNING, INFO, DEBUG};

int cLog(int lev, char *format, ...);

#ifdef OPEN_LOG
#define LOG(lev, format, args...)	printf((format), ##args)//cLog((lev), (format), args);
#else
#define LOG(lev, format, args...)
#endif

#endif //CLOG_H
