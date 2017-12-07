/*
 * gpio.h
 *
 * Created: 3-12-2017 19:10:38
 *  Author: Stan Geitel
 */ 


#ifndef GPIO_H_
#define GPIO_H_

void set_output_gpio(void);
void clear_output_gpio(void);
void enable_input_gpio(void);
void enable_pullup_gpio(void);
int read_gpio(void);

#endif /* GPIO_H_ */