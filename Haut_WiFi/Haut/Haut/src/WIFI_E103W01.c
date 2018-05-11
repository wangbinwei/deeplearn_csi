/*
 * WIFI_E103W01.c
 *
 * Created: 2018/3/21 22:59:18
 *  Author: Huo
 */ 
#include <asf.h>
#include <stdio.h>
#include "WIFI_E103W01.h"
#include "BLE_E104BT02.h"

uint8_t wifi_transmitbuff[22] = {20,
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
	//port_pin_toggle_output_level(LED_PIN);
	//port_pin_set_output_level(LED_PIN, true);
	//delay_s(1);
	//usart_write_buffer_job(&usart_wifi_instance, (uint8_t *)wifi_transmitbuff, 10);
	//usart_write_buffer_job(&usart_ble_instance, (uint8_t *)wifi_rx_buffer, 10);
	//usart_write_buffer_job(&usart_ble_instance,
	//(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}

static void usart_write_callback(struct usart_module *const usart_module)
{
	//port_pin_toggle_output_level(LED_PIN);
	//port_pin_set_output_level(LED_PIN, false);
}
//! [callback_funcs]


void configure_wifi_usart(void)
{
	struct usart_config wifi_configure_usart;
	usart_get_config_defaults(&wifi_configure_usart);

	wifi_configure_usart.baudrate    = 115200;
	wifi_configure_usart.mux_setting = USART_RX_1_TX_0_XCK_1;
	wifi_configure_usart.pinmux_pad0 = PINMUX_PA08C_SERCOM0_PAD0;//PINMUX_PA25C_SERCOM3_PAD3
	wifi_configure_usart.pinmux_pad1 = PINMUX_PA09C_SERCOM0_PAD1;
	wifi_configure_usart.pinmux_pad2 = PINMUX_UNUSED;
	wifi_configure_usart.pinmux_pad3 = PINMUX_UNUSED;

	while (usart_init(&usart_wifi_instance,SERCOM0, &wifi_configure_usart) != STATUS_OK) {
	}

	usart_enable(&usart_wifi_instance);
	
	//!other_pin_init
	struct port_config pin_config;
	port_get_config_defaults(&pin_config);
	
	pin_config.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(WIFI_VDD_PIN,&pin_config);
	wifi_power(true);
	port_pin_set_config(WIFI_SLEEPOUT_PIN,&pin_config);
	wifi_sleep(false);
	port_pin_set_config(WIFI_RST_PIN,&pin_config);
	wifi_reset(false);
			
}

void configure_wifi_callbacks(void)
{
	//! [setup_register_callbacks]
	usart_register_callback(&usart_wifi_instance,
	usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_wifi_instance,
	usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	//! [setup_register_callbacks]

	//! [setup_enable_callbacks]
	usart_enable_callback(&usart_wifi_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_wifi_instance, USART_CALLBACK_BUFFER_RECEIVED);
	//! [setup_enable_callbacks]
}

void wifi_power(bool mode)
{
	if (mode)
		port_pin_set_output_level(WIFI_VDD_PIN, true);
	else
		port_pin_set_output_level(WIFI_VDD_PIN, false);
}

void wifi_sleep(bool mode)
{
	if (mode)
		port_pin_set_output_level(WIFI_SLEEPOUT_PIN, false);
	else
		port_pin_set_output_level(WIFI_SLEEPOUT_PIN, true);
}

void wifi_reset(bool mode)
{
	if(mode)
		port_pin_set_output_level(WIFI_RST_PIN, false);
	else
		port_pin_set_output_level(WIFI_RST_PIN, true);
}
	
