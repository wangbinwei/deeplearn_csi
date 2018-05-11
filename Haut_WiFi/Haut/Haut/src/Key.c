/*
 * Key.c
 *
 * Created: 2018/4/15 15:05:49
 *  Author: Huo
 */ 
#include <asf.h>
#include "Key.h"

void configure_extint_channel(void)
{
	struct extint_chan_conf config_extint_buttonstart;
	extint_chan_get_config_defaults(&config_extint_buttonstart);
	config_extint_buttonstart.gpio_pin           = BUTTON_start_EIC_PIN;
	config_extint_buttonstart.gpio_pin_mux       = BUTTON_start_EIC_MUX;
	config_extint_buttonstart.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_buttonstart.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(BUTTON_start_EIC_LINE, &config_extint_buttonstart);
}
void configure_extint_callbacks(void)
{
	//! [setup_5]
	extint_register_callback(extint_BUTTON_start_detection_callback,	BUTTON_start_EIC_LINE,	EXTINT_CALLBACK_TYPE_DETECT);
	//! [setup_5]
	//! [setup_6]
	extint_chan_enable_callback(BUTTON_start_EIC_LINE,	EXTINT_CALLBACK_TYPE_DETECT);
	//! [setup_6]
}

//! [setup_7]
void extint_BUTTON_start_detection_callback(void)
{
	//port_pin_toggle_output_level(LED_PIN);
	//bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
	//port_pin_set_output_level(LED_0_PIN, pin_state);
}