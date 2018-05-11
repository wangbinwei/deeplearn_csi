/*
 * Finger.c
 *
 * Created: 2018/4/15 21:19:40
 *  Author: Huo
 */ 
#include <asf.h>
#include "FingerPrint.h"
#include "BLE_E104BT02.h"


uint8_t finger_transmitbuff[40] = {19,
	0xF1,0x1F,0xE2,0x2E,0xB6,0x6B,0xA8,0x8A,  //帧头
	0x00,0x00,								  //Dala Len
	0x00,									  //帧头校验
	0x00,0x00,0x00,0x00,					  //校验密码
	0x00,0x00,								  //CMD1&CMD2
	0x00,0x00,
	0xa5,0xa6,0xa7,0xa8,
	0xb5,0xb6,0xb7,0xb8,
};
static uint8_t FP_protocol_checksum(uint8_t *data,uint32_t length)
{
	uint32_t i = 0;
	uint8_t sum = 0;
	for(i=0;i<length;i++)
	sum+=data[i];
	return (uint8_t)((~sum)+1);
}
uint8_t FPaction_Flag;

void  FPaction(void)
{
	switch(FPaction_Flag){
		case FPaction_Register:
		break;
		case FPaction_CheckRegister:
		finger_transmitbuff[11] = 0x07;
		finger_transmitbuff[12] = FP_protocol_checksum(finger_transmitbuff+1,10);
		finger_transmitbuff[17] = 0x01;
		finger_transmitbuff[18] = 0x12;
		finger_transmitbuff[19] = FP_protocol_checksum(finger_transmitbuff+12,6);
		finger_transmitbuff[0] = 18;
		usart_write_buffer_job(&usart_finger_instance, finger_transmitbuff+1, finger_transmitbuff[0]);
		default:
		break;
	}
	FPaction_Flag = FPaction_No;
}

//! [callback_funcs]
static void usart_read_callback(struct usart_module *const usart_module)
{
	//port_pin_toggle_output_level(LED_PIN);
	usart_write_buffer_job(&usart_ble_instance, finger_rx_buffer, 25);
}

static void usart_write_callback(struct usart_module *const usart_module)
{
	//port_pin_toggle_output_level(LED_PIN);
}
static void extint_detection_callback(void)
{
	//port_pin_toggle_output_level(LED_PIN);
	//bool pin_state = port_pin_get_input_level(PIN_PA16);
	//port_pin_set_output_level(LED_PIN, pin_state);
	FPaction_Flag = FPaction_CheckRegister;
	FPaction();
	usart_read_buffer_job(&usart_finger_instance,(uint8_t *)finger_rx_buffer, 25);
}

void configure_FP(void)
{
	/**FP model EIC RST init**/
	struct extint_chan_conf config_extint_FPRST;
	extint_chan_get_config_defaults(&config_extint_FPRST);
	config_extint_FPRST.gpio_pin           = PIN_PA16A_EIC_EXTINT0;
	config_extint_FPRST.gpio_pin_mux       = MUX_PA16A_EIC_EXTINT0;
	config_extint_FPRST.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_FPRST.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(FP_EIC_LINE, &config_extint_FPRST);
		
	/**FP model EIC RST callback**/
	extint_register_callback(extint_detection_callback,	FP_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(FP_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
		
	/**FP model usart init**/
	struct usart_config finger_configure_usart;
	usart_get_config_defaults(&finger_configure_usart);

	finger_configure_usart.baudrate    = 115200;
	finger_configure_usart.mux_setting = USART_RX_3_TX_2_XCK_3;
	finger_configure_usart.pinmux_pad0 = PINMUX_UNUSED;
	finger_configure_usart.pinmux_pad1 = PINMUX_UNUSED;
	finger_configure_usart.pinmux_pad2 = PINMUX_PA18C_SERCOM1_PAD2;
	finger_configure_usart.pinmux_pad3 = PINMUX_PA19C_SERCOM1_PAD3;

	while (usart_init(&usart_finger_instance,
	SERCOM1, &finger_configure_usart) != STATUS_OK) {
	}
	usart_enable(&usart_finger_instance);
	
	/**FP model usart callback**/
	//! [setup_register_callbacks]
	usart_register_callback(&usart_finger_instance,usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_finger_instance,usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);

	usart_enable_callback(&usart_finger_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_finger_instance, USART_CALLBACK_BUFFER_RECEIVED);
		
	/**FP model other pin init **/
	struct port_config pin_config;
	port_get_config_defaults(&pin_config);
	
	pin_config.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(FP_RST_PIN,&pin_config);
	finger_reset(false);
}


void finger_reset(bool mode)
{
	if(mode)
	port_pin_set_output_level(FP_RST_PIN, false);
	else
	port_pin_set_output_level(FP_RST_PIN, true);
}
