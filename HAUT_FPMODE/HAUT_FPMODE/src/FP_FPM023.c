/*
 * Finger.c
 *
 * Created: 2018/4/15 21:19:40
 *  Author: Huo
 */ 
#include <asf.h>

bool FP_INT_FLAG = false;

uint8_t finger_transmitbuff[40] = {18,
	0xF1,0x1F,0xE2,0x2E,0xB6,0x6B,0xA8,0x8A,  //帧头
	0x00,0x07,								  //Dala Len
	0x86,									  //帧头校验
	0x00,0x00,0x00,0x00,					  //校验密码
	0x01,0x21,								  //CMD1&CMD2
	0xDE,
	0x00,
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
uint8_t INT_Flag = false;

void  FPaction(void)
{
	switch(FPaction_Flag){
		case FPaction_Register:
			finger_transmitbuff[10] = 0x08;
			finger_transmitbuff[11] = FP_protocol_checksum(finger_transmitbuff+1,10);
			finger_transmitbuff[16] = 0x01;
			finger_transmitbuff[17] = 0x11;
			finger_transmitbuff[18] = REG_IDX;
			finger_transmitbuff[19] = FP_protocol_checksum(finger_transmitbuff+12,7);
			finger_transmitbuff[0] = 19;
		usart_write_buffer_job(&usart_finger_instance, finger_transmitbuff+1, finger_transmitbuff[0]);
		break;
		case FPaction_CheckRegister:
			finger_transmitbuff[10] = 0x07;
			finger_transmitbuff[11] = FP_protocol_checksum(finger_transmitbuff+1,10);
			finger_transmitbuff[16] = 0x01;
			finger_transmitbuff[17] = 0x12;
			finger_transmitbuff[18] = FP_protocol_checksum(finger_transmitbuff+12,6);
			finger_transmitbuff[0] = 18;
			usart_write_buffer_job(&usart_finger_instance, finger_transmitbuff+1, finger_transmitbuff[0]);
		break;
		case FPaction_CheckMatch:
			finger_transmitbuff[10] = 0x07;
			finger_transmitbuff[11] = FP_protocol_checksum(finger_transmitbuff+1,10);
			finger_transmitbuff[16] = 0x01;
			finger_transmitbuff[17] = 0x21;
			finger_transmitbuff[18] = FP_protocol_checksum(finger_transmitbuff+12,6);
			finger_transmitbuff[0] = 18;
			usart_write_buffer_job(&usart_finger_instance, finger_transmitbuff+1, finger_transmitbuff[0]);
		break;
		case FPaction_CheckMatchResult:
			finger_transmitbuff[10] = 0x07;
			finger_transmitbuff[11] = FP_protocol_checksum(finger_transmitbuff+1,10);
			finger_transmitbuff[16] = 0x01;
			finger_transmitbuff[17] = 0x22;
			finger_transmitbuff[18] = FP_protocol_checksum(finger_transmitbuff+12,6);
			finger_transmitbuff[0] = 18;
			usart_write_buffer_job(&usart_finger_instance, finger_transmitbuff+1, finger_transmitbuff[0]);
		break;
		case FPaction_CheckFingerState:
			finger_transmitbuff[10] = 0x07;
			finger_transmitbuff[11] = FP_protocol_checksum(finger_transmitbuff+1,10);
			finger_transmitbuff[16] = 0x01;
			finger_transmitbuff[17] = 0x35;
			finger_transmitbuff[18] = FP_protocol_checksum(finger_transmitbuff+12,6);
			finger_transmitbuff[0] = 18;
			usart_write_buffer_job(&usart_finger_instance,finger_transmitbuff+1,finger_transmitbuff[0]);
		break;
		case FPaction_SaveRegister:
			finger_transmitbuff[10] = 0x09;
			finger_transmitbuff[11] = FP_protocol_checksum(finger_transmitbuff+1,10);
			finger_transmitbuff[16] = 0x01;
			finger_transmitbuff[17] = 0x13;
			//finger_transmitbuff[18] = ;//ID号
			//finger_transmitbuff[19] = ;//ID号
			finger_transmitbuff[20] = FP_protocol_checksum(finger_transmitbuff+12,8);
		break;
		default:
		break;
	}
	//FPaction_Flag = FPaction_No;
}

//! [callback_funcs]
static void usart_read_callback(struct usart_module *const usart_module)
{
	static REG_IDX = 1; //按压次数
	static ID_L=0; //ID号	
	switch(FPaction_Flag)
	{
		case FPaction_RegisterBack:
			
			//usart_write_buffer_job(&usart_ble_instance,finger_rx_buffer,22);
			if(finger_rx_buffer[20] == 0x00 && finger_rx_buffer[0] == 0xF1)  //注册完成
			{
				delay_ms(2); //不延时会有错误码 04
				FPaction_Flag = FPaction_CheckRegister;
				FPaction();
			}
			else if((finger_rx_buffer[21] == 0x04) && (finger_rx_buffer[0] == 0xF1)) //失败，系统忙
			{
				
			}
			
		break;
		case FPaction_CheckRegisterBack:
			//usart_write_buffer_job(&usart_ble_instance,finger_rx_buffer,25);
			if((finger_rx_buffer[20] == 0x00) && (finger_rx_buffer[0] == 0xF1))
			{
				usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,4);
				//delay_s(1);
			}
			else if(finger_rx_buffer[20] == 0x04 && finger_rx_buffer[0] == 0xF1) //完成码为0x00000004,sensor还没有处理完指纹数据,延时200ms
			{
				delay_ms(200);
				FPaction_Flag = FPaction_CheckRegister;
				FPaction();
			}
			else if(finger_rx_buffer[0] == 0x00)  //注册成功
			{
				if(PROC > finger_rx_buffer[2] && finger_rx_buffer[2] < 0x64)
				{
					usart_write_buffer_job(&usart_ble_instance,finger_rx_buffer,4);
					delay_ms(200);
					PROC = finger_rx_buffer[2];
					delay_ms(200);
					FPaction_Flag = FPaction_CheckFingerState;
					FPaction();					
				} 
				else if(finger_rx_buffer[2] >= 0x64) //保存指纹 
				{
					FPaction_Flag = FPaction_SaveRegister;
					FPaction();
				}
				//usart_write_buffer_job(&usart_ble_instance,finger_rx_buffer,4);
			}
			
		break;
		
		case FPaction_CheckMatchBack: 
			FPaction_Flag = FPaction_CheckMatchResult;
			FPaction();
		break;
		case FPaction_CheckMatchResultBack:
			if((finger_rx_buffer[20]  == 0)&&(finger_rx_buffer[0] == 0xF1))
			{
				usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,7);
			}
			else if((finger_rx_buffer[20]  == 0x0A)&&(finger_rx_buffer[0] == 0xF1))
			{
				usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,7);
			}
			else if((finger_rx_buffer[20]  == 0x04)&&(finger_rx_buffer[0] == 0xF1))
			{
				usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,1);
			}
			else if(finger_rx_buffer[0] == 0x00)
				usart_write_buffer_job(&usart_ble_instance, finger_rx_buffer, 7);
				
		break;
		case FPaction_CheckFingerStateBack:
			 //usart_write_buffer_job(&usart_ble_instance,finger_rx_buffer,23);
			 if(finger_rx_buffer[20] != 0x00 || finger_rx_buffer[21] ==0x01 ) //提示手指移开sensor再次按压
			 {
				 delay_ms(200);
				 usart_write_buffer_job(&usart_ble_instance,finger_rx_buffer,23);
			 }
			 else if(finger_rx_buffer[20] == 0x00 && finger_rx_buffer[21] ==0x00 && REG_IDX <=6)
			 {
				 REG_IDX++;
				 delay_s(5);
				 FPaction_Flag = FPaction_Register;
				 FPaction();
			 }
		break;
		default:
		break;
	}
	
	
}

static void usart_write_callback(struct usart_module *const usart_module)
{
	//port_pin_toggle_output_level(LED_PIN);
	switch(FPaction_Flag)
	{
		case FPaction_Register:
			FPaction_Flag = FPaction_RegisterBack;
			usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,22);
		break;
		case FPaction_CheckRegister:
			FPaction_Flag = FPaction_CheckRegisterBack;
			usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,21); //长度21到错误码
		break;
		case FPaction_CheckMatch:
			FPaction_Flag = FPaction_CheckMatchBack;
			usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,22);
		break;
		case FPaction_CheckMatchResult:
			FPaction_Flag = FPaction_CheckMatchResultBack;
			usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,21);
		break;
		case FPaction_CheckFingerState:
			FPaction_Flag = FPaction_CheckFingerStateBack;
			usart_read_buffer_job(&usart_finger_instance,finger_rx_buffer,23);
		break;
		default:
		break;
	}
	
}
static void extint_detection_callback(void)
{
	port_pin_toggle_output_level(LED_PIN);
	//if(!FP_INT_FLAG)
		//FP_INT_FLAG = true;
	
	//bool pin_state = port_pin_get_input_level(PIN_PA16);
	//port_pin_set_output_level(LED_PIN, false);
	//FPaction_Flag = FPaction_CheckMatch;
	//FPaction();
	
}

void configure_FP(void)
{
	/**FP model EIC INT init**/
	struct extint_chan_conf config_extint_FPINT;
	extint_chan_get_config_defaults(&config_extint_FPINT);
	config_extint_FPINT.gpio_pin           = PIN_PA16A_EIC_EXTINT0;
	config_extint_FPINT.gpio_pin_mux       = MUX_PA16A_EIC_EXTINT0;
	config_extint_FPINT.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_FPINT.detection_criteria = EXTINT_DETECT_FALLING;
	extint_chan_set_config(FP_EIC_LINE, &config_extint_FPINT);
		
	/**FP model EIC INT callback**/
	extint_register_callback(extint_detection_callback,	FP_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(FP_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
		
	/**FP model usart init**/
	struct usart_config finger_configure_usart;
	usart_get_config_defaults(&finger_configure_usart);

	finger_configure_usart.baudrate    = 115200;
	finger_configure_usart.mux_setting = USART_RX_3_TX_2_XCK_3;
	finger_configure_usart.pinmux_pad0 = PINMUX_UNUSED;
	finger_configure_usart.pinmux_pad1 = PINMUX_UNUSED;
	finger_configure_usart.pinmux_pad2 = PINMUX_PA18C_SERCOM1_PAD2;//PINMUX_PA18C_SERCOM1_PAD2
	finger_configure_usart.pinmux_pad3 = PINMUX_PA19C_SERCOM1_PAD3;//PINMUX_PA19C_SERCOM1_PAD3

	while (usart_init(&usart_finger_instance,SERCOM1, &finger_configure_usart) != STATUS_OK) {}
	usart_enable(&usart_finger_instance);
	
	/**FP model usart callback**/
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
	
	//pin_config.direction = PORT_PIN_DIR_INPUT;
	//port_pin_set_config(FP_INT_PIN, &pin_config);
}


void finger_reset(bool mode)
{
	if(mode)
	port_pin_set_output_level(FP_RST_PIN, false);
	else
	port_pin_set_output_level(FP_RST_PIN, true);
}

