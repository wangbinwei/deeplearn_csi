/*
 * Finger.h
 *
 * Created: 2018/4/15 21:19:55
 *  Author: Huo
 */ 


#ifndef FP_FPM023_H_
#define FP_FPM023_H_
#include <asf.h>

extern uint8_t finger_transmitbuff[40];
uint8_t finger_rx_buffer[26];

struct usart_module usart_finger_instance;

typedef enum{
	FPaction_No = 0,
	FPaction_Register,
	FPaction_RegisterBack,
	FPaction_CheckRegister,
	FPaction_CheckRegisterBack,
	FPaction_SaveRegister,
	FPaction_CheckSaveRegister,
	FPaction_CancelRegister,
	FPaction_UpdateStorage,
	FPaction_CheckUpdateStorage,
	FPaction_Match,
	FPaction_CheckMatch,
	FPaction_CheckMatchBack,
	FPaction_CheckMatchResult,
	FPaction_CheckMatchResultBack,
	FPaction_Delate,
	FPaction_CheckDelate,
	FPaction_CheckFingerState,
	FPaction_CheckFingerStateBack,
	}FPaction_list; 

extern uint8_t FPaction_Flag;

void  FPaction(void);

void configure_FP(void);
void finger_reset(bool mode);
void Register();

#endif /* FINGER_H_ */