#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include "uart.h"

#include <string.h>
#include <sys/ioctl.h>

int sfd;

void open_uart(){
  sfd = open("/dev/serial0", O_RDWR | O_NOCTTY);
  if(sfd == -1){
    printf("Error no is: %d.\n", errno);
    exit(-1);
  }
  struct termios options;
  tcgetattr(sfd, &options);

  cfsetspeed(&options, B9600);
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
  options.c_cflag |= PARENB;
  options.c_cflag &= ~PARODD;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag |= CLOCAL;
  options.c_cflag |= CREAD;
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_oflag |= OPOST;
  options.c_cc[VTIME] = 0;
  options.c_cc[VMIN] = 0;
  tcsetattr(sfd, TCSANOW, &options);
  printf("serial0 opened\n");

  tcflush(sfd, TCIOFLUSH);
}

void close_uart(){
  close(sfd);
}

int check_data(){
  int bytes;
  ioctl(sfd, FIONREAD, &bytes);
  return(bytes);
}

uint8_t rx_uart(){
  uint8_t data;
  while(!(check_data()));
  read(sfd, &data, 1);
  return(data);
}

void tx_uart(uint8_t data){
  write(sfd, &data, 1);
}
