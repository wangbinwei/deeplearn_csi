/*
 * Key.h
 *
 * Created: 2018/4/15 15:06:10
 *  Author: Huo
 */ 


#ifndef KEY_H_
#define KEY_H_

#define EXTINT_CLOCK_SOURCE      GCLK_GENERATOR_0

void configure_extint_channel(void);
void configure_extint_callbacks(void);
void extint_BUTTON_start_detection_callback(void);


#endif /* KEY_H_ */