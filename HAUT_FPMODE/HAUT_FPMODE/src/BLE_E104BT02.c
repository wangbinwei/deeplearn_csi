/*
 * BLE_E104BT02.c
 *
 * Created: 2018/3/21 22:59:18
 *  Author: Huo
 */ 
#include <asf.h>
#include <stdio.h>
#include "BLE_E104BT02.h"

static bool BLE_protocol_checksum(void);

uint8_t ble_transmitbuff[21] = {20,
	0xAA,0x55,
	0x82,
	0x84,
	0x85,0x86,0x87,0x88,
	0x95,0x96,0x97,0x98,
	0xa5,0xa6,0xa7,0xa8,
	0xb5,0xb6,0xb7,0xb8,
};

//! [callback_funcs]
static void usart_read_callback(struct usart_module *const usart_module)
{
	switch(ble_rx_buffer[3])
	{
		case BLE_ID_synClock:
		break;
		case BLE_ID_CommunicationOut:
		break;
		default:
		break;
	}
	//port_pin_toggle_output_level(LED_PIN);
	//port_pin_set_output_level(LED_PIN, true);
	//delay_s(1);
	//usart_write_buffer_job(&usart_ble_instance, (uint8_t *)ble_transmitbuff, 20);
	
	//usart_write_buffer_job(&usart_ble_instance,
	//(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}

static void usart_write_callback(struct usart_module *const usart_module)
{
	//port_pin_toggle_output_level(LED_PIN);
	//port_pin_set_output_level(LED_PIN, false);
}
//! [callback_funcs]


void configureBLE_usart(void)
{
	struct usart_config ble_configure_usart;
	usart_get_config_defaults(&ble_configure_usart);
	
	ble_configure_usart.baudrate    = 19200;
	ble_configure_usart.mux_setting = USART_RX_3_TX_2_XCK_3;
	ble_configure_usart.pinmux_pad0 = PINMUX_UNUSED;
	ble_configure_usart.pinmux_pad1 = PINMUX_UNUSED;
	ble_configure_usart.pinmux_pad2 = PINMUX_PA24C_SERCOM3_PAD2;
	ble_configure_usart.pinmux_pad3 = PINMUX_PA25C_SERCOM3_PAD3;

	while (usart_init(&usart_ble_instance,SERCOM3, &ble_configure_usart) != STATUS_OK) {}

	usart_enable(&usart_ble_instance);
	
	//! [setup_register_callbacks]
	usart_register_callback(&usart_ble_instance,usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_ble_instance,usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);

	usart_enable_callback(&usart_ble_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_ble_instance, USART_CALLBACK_BUFFER_RECEIVED);
	//! [setup_enable_callbacks]
	
	//!other_pin_init
	struct port_config pin_config;
	port_get_config_defaults(&pin_config);
	
	pin_config.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(BLE_VDD_PIN,&pin_config);
	ble_power(true);
	delay_ms(200);
	port_pin_set_config(BLE_SLEEP_PIN,&pin_config);
	ble_sleep(false);
	delay_ms(30);
	port_pin_set_config(BLE_RST_PIN,&pin_config);
	ble_reset(false);
		
	pin_config.direction = PORT_PIN_DIR_INPUT;
	port_pin_set_config(BLE_DATATI_PIN,&pin_config);
	port_pin_set_config(BLE_STATUE_PIN,&pin_config);
	
}


//PIN_PB22
void ble_power(bool mode)
{
	if (mode)
		port_pin_set_output_level(BLE_VDD_PIN, true);
	else
		port_pin_set_output_level(BLE_VDD_PIN, false);
}

void ble_sleep(bool mode)
{
	if (mode)
		port_pin_set_output_level(BLE_SLEEP_PIN, true);
	else
		port_pin_set_output_level(BLE_SLEEP_PIN, false);
}

void ble_reset(bool mode)
{
	if(mode)
		port_pin_set_output_level(BLE_RST_PIN, true);
	else
		port_pin_set_output_level(BLE_RST_PIN, false);
}
	
bool ble_is_data_transmission(void)
{
	if (port_pin_get_input_level(BLE_DATATI_PIN))
	{
		return false;
	} 
	else
	{
		return true;
	}
}

bool ble_con_state(void)
{
	if (port_pin_get_input_level(BLE_STATUE_PIN))
	{
		return false;
	}
	else
	{
		return true;
	}
}
static bool BLE_protocol_checksum(void)
{
	//ble_transmitbuff
	return true;
}