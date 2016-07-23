topDir=$(shell pwd)
CC=gcc
#CC = arm-hisiv100nptl-linux-gcc

# compile option
INCLUDE_FLAG = -I$(topDir)/include/
LIB_FLAG = -levent -L$(topDir)/lib
MACRO = -DOPEN_LOG
C_FLAG = -Wall #-fno-stack-protector
C_FLAG += $(MACRO) $(INCLUDE_FLAG) $(LIB_FLAG)

srvSubObjs = server.o cLog.o common.o

all: server log common
	cd $(topDir)/src && \
	$(CC) -o srv $(srvSubObjs) $(C_FLAG) && \
	mv srv ../

server: log common 
	cd $(topDir)/src && \
	$(CC) server.c -c $(C_FLAG)

common: log
	cd $(topDir)/src && \
	$(CC) common.c -c $(C_FLAG)

log:
	cd $(topDir)/src && \
	$(CC) cLog.c -c

client: log common
	cd $(topDir)/cli && \
	$(CC) client.c -o client $(C_FLAG) && \
	mv client ../

clean:
	cd $(topDir) && rm srv client && \
	cd $(topDir)/src && rm *.o
