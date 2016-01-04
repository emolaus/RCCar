#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int openSocket();
int closeSocket();
int checkCommand();
char buf[100];
