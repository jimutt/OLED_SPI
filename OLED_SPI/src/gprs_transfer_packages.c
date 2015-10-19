/*
 * gprs_transfer_packages.c
 *
 * Created: 2015-10-19 14:51:24
 *  Author: jiut0001
 */ 
#include "gprs_transfer_packages.h"

uint8_t num_to_char_count(char data_type, void *num) {
	
}

gprs_send_data_log() {
	char send_string[HTTP_PACKAGE_STRING_LENGTH];
	uint16_t pos = 0;
	uint8_t nDigits;
	
	//för att hitta antalet bokstäver i ett nummer:
	if(the_integer == 0) {
		nDigits = 1;
	}
	else {
		nDigits = floor(log10(abs(the_integer))) + 1;		
	}

	
	while(gprs_log_buf.head != gprs_log_buf.head) {
		//Add entry to json object
	}
}

void gprs_send_buf_init(gprs_send_buffer *buf) {
	buf->len = 16;
	buf->tail = 0;
	buf->head = 0;
}

void gprs_buf_push(log_entry entry, gprs_send_buffer *buf) {
	buf->data->entries[buf->head] = value;
	buf->head++;
	if(buf->head == buf->len) buf->head = 0;
}