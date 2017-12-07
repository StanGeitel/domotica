/*
 * gpio.h
 *
 * Created: 3-12-2017 19:10:38
 *  Author: Stan Geitel
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#define PIN(port) _SFR_IO8(0x019 - ((port - 10) * 3))
#define DDR(port) _SFR_IO8(0x01A - ((port - 10) * 3))
#define PORT(port) _SFR_IO8(0x01B - ((port - 10) * 3))

void set_output_gpio(unsigned char port, int pin);
void clear_output_gpio(unsigned char port, int pin);
void enable_input_gpio(unsigned char port, int pin);
void enable_pullup_gpio(unsigned char port, int pin);
int read_gpio(unsigned char port, int pin);
void init_interrupt_gpio(void);
void init_external_interrupt0_gpio(void);

#endif /* GPIO_H_ */