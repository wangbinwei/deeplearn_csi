/*
 * Finger.h
 *
 * Created: 2018/4/15 21:19:55
 *  Author: Huo
 */ 


#ifndef FINGERPRINT_H_
#define FINGERPRINT_H_

extern uint8_t finger_transmitbuff[40];
volatile uint8_t finger_rx_buffer[26];

struct usart_module usart_finger_instance;

typedef enum{
	FPaction_No = 0,
	FPaction_Register,
	FPaction_CheckRegister,
	FPaction_SaveRegister,
	FPaction_CheckSaveRegister,
	FPaction_CancelRegister,
	FPaction_UpdateStorage,
	FPaction_CheckUpdateStorage,
	FPaction_Match,
	FPaction_CheckMatch,
	FPaction_Delate,
	FPaction_CheckDelate,
	}FPaction_list; 

void configure_FP(void);
void finger_reset(bool mode);

#endif /* FINGER_H_ */