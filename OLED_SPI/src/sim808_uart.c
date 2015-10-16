/*
 * sim808_uart.c
 *
 * Created: 2015-10-16 09:09:14
 *  Author: jiut0001
 */ 

#include "sim808_uart.h"

void sim808_init() {
	uint8_t success;
	
	do {
		success = 1;
		sim808_send_command(CMD_RESET);
		sim808_send_command(CMD_NO_ECHO);	//Disable echo
		usart_register_callback(&SIM808_usart, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
		usart_read_buffer_job(&SIM808_usart, (uint8_t *)incoming_byte, MAX_RX_BUFFER_LENGTH);
		sim808_parse_response_wait();
		sim808_send_command(CMD_GPS_PWR_ON);	//Enable GPS
		sim808_parse_response_wait();
		delay_ms(200);
		success = sim808_init_gprs();					//Initiate GPRS
	} while(success == 0);

}

void sim808_reset() {
	port_pin_set_output_level(SIM808_RESET_PIN, true);
	delay_ms(500);
	port_pin_set_output_level(SIM808_RESET_PIN, false);
	delay_ms(6000);
}

uint8_t sim808_init_gprs() {
	volatile uint8_t res;
	command cmd;
	cmd.expected_response = "OK";
	cmd.callback_enabled = 0;

	cmd.cmd = "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"";
	sim808_send_command(cmd);
	if(!sim808_parse_response_wait()) res = 0;
				
	cmd.cmd = "AT+SAPBR=3,1,\"APN\",\"online.telia.se\"";
	sim808_send_command(cmd);
	if(!sim808_parse_response_wait()) res = 0;
				
	cmd.cmd = "AT+SAPBR=1,1";
	sim808_send_command(cmd);
	if(!sim808_parse_response_wait()) res = 0;
				
	cmd.cmd = "AT+HTTPINIT";
	sim808_send_command(cmd);
	if(!sim808_parse_response_wait()) res = 0;
				
	cmd.cmd = "AT+HTTPPARA=\"CID\",1"; 		 //Bearer profile identifier
	sim808_send_command(cmd);
	if(!sim808_parse_response_wait()) res = 0;
				
	cmd.cmd = "AT+HTTPPARA=\"UA\",\"FONA\""; //User agent
	sim808_send_command(cmd);
	if(!sim808_parse_response_wait()) res = 0;
				
	cmd.cmd = "AT+HTTPPARA=\"URL\",\"http://tripcomputer.azurewebsites.net/api/test/1\"";
	sim808_send_command(cmd);
	if(!sim808_parse_response_wait()) res = 0;
				
	cmd.cmd = "AT+HTTPPARA=\"TIMEOUT\",30";
	sim808_send_command(cmd);
	if(!sim808_parse_response_wait()) res = 0;

	return res;
	
}

void sim808_send_command(command cmd) {
	//uint8_t send_string_len = strlen(cmd.cmd)+2;
	//char send_string[50];
	last_command = cmd;
	printf("%s\r\n", cmd.cmd);
}

uint8_t sim808_parse_response_wait() {
	while(SIM808_buf.available != 1);	
	return sim808_parse_response();
}

uint8_t sim808_parse_response() {
	volatile uint8_t result = 0;
	volatile uint8_t cmp_cmd_len = SIM808_buf.position;
	volatile uint8_t resp_len = strlen(last_command.expected_response);
	
	volatile char originalChar = SIM808_buf.command[resp_len];
	if(cmp_cmd_len > resp_len) {
		SIM808_buf.command[resp_len] = '\0';
	}
	
	if(strcmp(SIM808_buf.command, last_command.expected_response) == 0) {
		result = 1;
	}
	
	SIM808_buf.command[resp_len] = originalChar;	//Reset to original state after comparison
	
	if(last_command.callback_enabled) {
		(*last_command.response_cb)(result, SIM808_buf.command);	
	}
	
	SIM808_buf.available = 0;
	volatile testVar = SIM808_buf.position;
	SIM808_buf.position = 0;
	memset(SIM808_buf.command, 0, sizeof(unsigned char)*COMMAND_BUFFER_SIZE);
	
	return result;
}

void usart_read_callback(struct usart_module *const usart_module)
{
	if(incoming_byte[0] == '\n') {
		if(SIM808_buf.position > 1) {
			SIM808_buf.command[SIM808_buf.position] = '\0';
			SIM808_buf.available = 1;
		}
	}
	else if(incoming_byte[0] != '\r'){
		SIM808_buf.command[SIM808_buf.position] = incoming_byte[0];
		SIM808_buf.position++;
	}
	
	usart_read_buffer_job(&SIM808_usart, (uint8_t *)incoming_byte, MAX_RX_BUFFER_LENGTH);
}

void usart_write_callback(struct usart_module *const usart_module)
{
	//Write callback
}

void init_SIM808_uart(void) {
	struct usart_config uart_settings;
	usart_get_config_defaults(&uart_settings);
	
	//Set clock to 8M
	uart_settings.generator_source = GCLK_GENERATOR_0;
	
	uart_settings.mux_setting = USART_RX_1_TX_2_XCK_3;
	uart_settings.pinmux_pad0 = PINMUX_PA10C_SERCOM0_PAD2; // Tx
	uart_settings.pinmux_pad1 = PINMUX_PA09C_SERCOM0_PAD1; // Rx
	uart_settings.pinmux_pad2 = PINMUX_UNUSED;
	uart_settings.pinmux_pad3 = PINMUX_UNUSED;
	uart_settings.baudrate = 9600;
	while (usart_init(&SIM808_usart, SERCOM0, &uart_settings) != STATUS_OK){}
	
	stdio_serial_init(&SIM808_usart, SERCOM0, &uart_settings);
	usart_enable(&SIM808_usart);
}

void init_sim808_usart_callbacks(void)
{
	usart_register_callback(&SIM808_usart, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	//usart_register_callback(&SIM808_usart, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&SIM808_usart, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&SIM808_usart, USART_CALLBACK_BUFFER_RECEIVED);
}