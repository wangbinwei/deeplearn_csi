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

extern bool FP_INT_FLAG;

void LedInit(void)
{
	struct port_config pin_config;
	port_get_config_defaults(&pin_config);
	
	pin_config.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LED_PIN,&pin_config);
	port_pin_set_output_level(LED_PIN,false);
}

int main (void)
{
	system_init();
	LedInit();
	delay_init();
	delay_ms(300);
	configure_FP();
	configureBLE_usart();
	//uint8_t string[] = "AT+SAVETRANSLINK=1,\"192.168.1.20\",1001,\"TCP\",10\n";
	Register();
	while(1)
	{
		//port_pin_toggle_output_level(LED_PIN);
		//delay_s(12);	
	}

	/* Insert application code here, after the board has been initialized. */
}
