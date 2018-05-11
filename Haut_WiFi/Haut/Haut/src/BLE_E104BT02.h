/*
 * BLE_E104BT02.h
 *
 * Created: 2018/4/4 22:34:10
 *  Author: Huo
 */ 


#ifndef BLE_E104BT02_H_
#define BLE_E104BT02_H_

//! [module_inst]
struct usart_module usart_ble_instance;
//! [module_inst]
//! [rx_buffer_var]
volatile uint8_t ble_rx_buffer[21];
extern uint8_t ble_transmitbuff[21];
//! [rx_buffer_var]


void configureBLE_usart(void);
void configureBLE_usart_callbacks(void);
void ble_power(bool mode);
void ble_sleep(bool mode);
void ble_reset(bool mode);
bool ble_is_data_transmission(void);
bool ble_con_state(void);



#endif /* BLE_E104BT02_H_ */