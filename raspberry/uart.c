#include "uart.h"
#include "rpi.h"

unsigned char rx_data[8];
unsigned char tx_data[8] = {0, 1, 2, 3, 4, 5, 6, 7};

void poll_uart(){
    struct pollfd fds[1];
    fds[0].fd = open("/dev/serial0", O_RDWR | O_NOCTTY);
    fds[0].events = POLLIN ;
    int pollrc = poll( fds, 1, 1000);
    if (pollrc < 0)
    {
        perror("poll");
    }
    else if( pollrc > 0){
        if( fds[0].revents & POLLIN )
        {
            char buff[1024];
            ssize_t rc = read(serial_fd, buff, sizeof(buff) );
            if (rc > 0)
            {
                /* You've got rc characters. do something with buff */
            }
        }
    }
  }

void init_uart(){

  UART_IBRD &= ~(0xFFFF); //all 16 bits low
  UART_IBRD |= BAUDDIV;   //write BAUDDIV to register resulting in 9600 baudrate

  UART_LCRH &= ~(0xFF);   //all 8 bits low
  UART_LCRH |= (3 << 5);  //set word lenght to 8 bits
  UART_LCRH |= (1 << 2);  //select even parity
  UART_LCRH |= (1 << 1);  //enable parity
/*  UART_IMSC |= (0xF1 << 6); //mask all interrupts
  UART_IMSC &= ~(3 << 4);   //except rx and tx
  UART_IMSC |= (1 << 1);
  UART_ICR |= (3 << 4);   //clear interrupts on rx and tx
*/
  UART_CR &= ~(0xFFFF);   //set all 16 bits low
  UART_CR |= (1 << 9);    //enable rx
  UART_CR |= (1 << 8);    //enable tx
  UART_CR |= (1 << 0);    //uart enabled

}

void loop_uart(){
  while(1){
    if(UART_FR & (1 << 6)){
      read_uart();
    }
  }
}

void read_uart(){
  char c = 0;
  c = UART_DR & 0xFF;
//  while((UART_DR & (1 << 11)) || (UART_FR & (1 << 4)));
  if(UART_RSRECR & (1 << 3) || UART_RSRECR & (1 << 2) || UART_RSRECR & (1 << 1) || UART_RSRECR & (1 << 0)){
    //send_nack();
    printf("error: flag set.\n");
    exit(-1);
  }
  //send_ack();
  printf("Recieved: %d\n", c);
  //data[i] = UART_DR & 0xFF;
  //i++;
}

void write_uart(){
  char data = 65;
  UART_DR |= data;
  printf("Send: %d\n", data);
}


void send_ack(){

}

void send_nack(){

}


/*
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
}

void close_uart(){
  close(sfd);
}

void rx_uart(){
  char c;
  int count;
  while(1){
    count = read(sfd, &c, 1);
    if(count!=0){
      printf("rx: %d\n", c);
    }
  }
  int bytes = 0;
  for(int i = 0; i < 8; i++){
    ioctl(sfd, FIONREAD, &bytes);
    printf("bytes: %d\n", bytes);
    if(bytes != 0){
      read(sfd, &rx_data[i], 1);
  //    usleep(PAUSE_LENGHT);
      printf("rx: %d\n", rx_data[i]);
    }
  }

}

void tx_uart(){
  char c = 'A';
  int count;
  while(1){
    count = write(sfd, &c, sizeof(c));
    printf("tx: %d\n", c);
  }

  for(int i = 0; i < 8; i++){
    write(sfd, &tx_data[i], 1);
    usleep(PAUSE_LENGHT);
    printf("tx: %d\n", tx_data[i]);
  }

}
*/
