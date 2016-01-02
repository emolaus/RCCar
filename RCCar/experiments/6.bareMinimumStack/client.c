#include "client.h"

char *socket_path = "./socket";
//char *socket_path = "\0hidden";
struct sockaddr_un addr;
char buf[100];
int fd,rc, cl;
// select stuff
fd_set readset;
int result, iof = -1;
struct timeval timesetting;

int openSocket() {
  
  printf("Connecting to socket... ");
  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    return -1;
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    return -1;
  }
  return 0;
}
int closeSocket() {
  close(fd);
}
/*
 * TODO parse command from char buf to int 
 * */
int checkCommand() {
  // TODO check if file descriptor ready to read
  //printf("Calling select... ");
  
  // Initialize the set
  FD_ZERO(&readset);
  FD_SET(fd, &readset);
  // Setup for select() call  
  timesetting.tv_sec = 0;
  timesetting.tv_usec = 5000;
  result = select(fd+1, &readset, NULL, NULL, &timesetting);
  //printf("Returned with result %d. timesetting: sec: %d, usec: %d\n", result, (int) timesetting.tv_sec, (int) timesetting.tv_usec);
  
  if (result > 0 && FD_ISSET(fd, &readset)) {
    if ( (rc=read(fd,buf,sizeof(buf))) > 0) {
      //printf("read %u bytes: %.*s\n", rc, rc, buf);
      //int i;
      //for (i = 0; i < rc; i++) printf("%d\n", (int) buf[i]);
      int intCommand = 0;
      intCommand += buf[0] * 1000;
      intCommand += buf[1] * 100;
      intCommand += buf[2] * 10;
      intCommand += buf[3];
      return intCommand;
    }
    if (rc == -1) {
      perror("read");
      return -1;
    }
    else if (rc == 0) {
      printf("EOF\n");
      close(fd);
      return -2;
    }
  } else return -3;
}
