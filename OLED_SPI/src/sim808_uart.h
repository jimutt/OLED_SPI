/*
 * sim808_uart.h
 *
 * Created: 2015-10-16 09:08:59
 *  Author: jiut0001
 */ 


#ifndef SIM808_UART_H_
#define SIM808_UART_H_

/*
 * sim808_uart.c
 *
 * Created: 2015-10-16 09:09:14
 *  Author: jiut0001
 */ 

#include <asf.h>

#define MAX_RX_BUFFER_LENGTH	1
#define COMMAND_BUFFER_SIZE		128
#define SIM808_RESET_PIN		PIN_PA06
volatile uint8_t incoming_byte[MAX_RX_BUFFER_LENGTH];

struct usart_module SIM808_usart;

typedef struct {
	unsigned char command[COMMAND_BUFFER_SIZE];
	uint8_t position;
	volatile uint8_t available;
} command_buffer;

command_buffer SIM808_buf;

typedef struct {
	char *cmd;
	char *expected_response;
	uint8_t callback_enabled;
	void (*response_cb)(uint8_t, char*);
} command;

command last_command;

command CMD_RESET;
command CMD_NO_ECHO;
command CMD_GPS_PWR_ON;
command CMD_GPS_PWR_OFF;

command CMD_GET_GPS_DATA;

void sim808_init(void);
void sim808_reset();
uint8_t	sim808_init_gprs();
void init_SIM808_uart(void);
void sim808_send_command(command);
uint8_t sim808_parse_response_wait(void);
uint8_t sim808_parse_response(void);
void usart_read_callback(struct usart_module *const);
void usart_write_callback(struct usart_module *const);



void init_sim808_usart_callbacks(void);


#endif /* SIM808_UART_H_ */