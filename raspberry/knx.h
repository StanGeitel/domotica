#ifndef KNX_H_
#define KNX_H_

#define PI_ADDRESS
#define SW_ADDRESS
#define DIM_ADDRESS

void init_knx();
void run_knx();
void send_telegram();
void recieve_telegram();
void send_ack();
void send_nack();
void check_des();

#endif
