/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "tc_timer.h"
#include "BLE_E104BT02.h"
#include "WIFI_E103W01.h"
#include "Key.h"
#include "FingerPrint.h"



void BuzzerInit(void)
{
	struct port_config pin_config;
	port_get_config_defaults(&pin_config);
		
	pin_config.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(BUZZER_PIN,&pin_config);
	port_pin_set_output_level(BUZZER_PIN,false);
}

void LedInit(void)
{
	struct port_config pin_config;
	port_get_config_defaults(&pin_config);
	
	pin_config.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_PIN,&pin_config);
	port_pin_set_output_level(LED_PIN,true);
	
}


int main (void)
{
	system_init();
	//BuzzerInit();
	LedInit();
	delay_init();

	//configure_tc();
	//configure_tc_callbacks();
	//
	//configureBLE_usart();
	//configureBLE_usart_callbacks();
	configure_wifi_usart();
	configure_wifi_callbacks();
	//configure_FP();
	
	
	//! [main]
	//! [enable_global_interrupts]
	//system_interrupt_enable_global();
	//! [enable_global_interrupts]
	while(1){
		//usart_read_buffer_job(&usart_wifi_instance,(uint8_t *)wifi_rx_buffer, 10);
		//////port_pin_set_output_level(BUZZER_PIN, true);
		//port_pin_set_output_level(LED_PIN, true);
		//port_pin_toggle_output_level(LED_PIN);
		//delay_s(1);
		//////port_pin_set_output_level(BUZZER_PIN, false);
		//port_pin_set_output_level(LED_PIN, false);
		//delay_s(1);
	}
	/* Insert application code here, after the board has been initialized. */
}