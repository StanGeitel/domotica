#include "uart.h"
#include "rpi.h"

int sfd;
unsigned char data[8];
int i = 0;

void open_uart(){
  sfd = open("/dev/serial0", O_RDWR | O_NOCTTY);
  if(sfd == -1`){
    printf("Error no is: %d.\n", errno);
    printf("Error description is: %s.\n", sterror(errno));
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

  tcsetattr(sfd, TCSANOW, &options);

}

void rx_uart(){

}

void tx_uart(){
  tcsendbreak(sfd, );

}











void init_uart(){
  UART_IBRD &= ~(0xFFFF); //all 16 bits low
  UART_IBRD |= BAUDDIV;   //write BAUDDIV to register resulting in 9600 baudrate

  UART_LCRH &= ~(0xFF);   //all 8 bits low
  UART_LCRH |= (3 << 5);  //set word lenght to 8 bits
  UART_LCRH |= (1 << 2);  //select even parity
  UART_LCRH |= (1 << 1);  //enable parity

  UART_IMSC |= (0xF1 << 6); //mask all interrupts
  UART_IMSC &= ~(3 << 4);   //except rx and tx
  UART_IMSC |= (1 << 1);
  UART_ICR |= (3 << 4);   //clear interrupts on rx and tx


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
//  while((UART_DR & (1 << 11)) || (UART_FR & (1 << 4)));
  if(UART_RSRECR & (1 << 3) || UART_RSRECR & (1 << 2) || UART_RSRECR & (1 << 1) || UART_RSRECR & (1 << 0)){
    send_nack();
    return(-1);
  }
  send_ack();

  data[i] = UART_DR & 0xFF;
  i++;
}

void write_uart(unsigned char data){
  UART_DR |= data;
}
