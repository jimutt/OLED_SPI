/*
 * gprs_transfer_packages.h
 *
 * Created: 2015-10-19 14:51:14
 *  Author: jiut0001
 */ 


#ifndef GPRS_TRANSFER_PACKAGES_H_
#define GPRS_TRANSFER_PACKAGES_H_

#define	HTTP_PACKAGE_STRING_LENGTH	2300; //ca 30 paket

#include "globals.h"

typedef struct {
	char time[6];
	float lat;
	float lng;
	float speed;
	uint8_t incliniation;
	float g_force;
} log_entry;

typedef struct {
	char date_time[12];
	uint32_t device;
	log_entry entries[255];
} data_log;

typedef struct {
	data_log data;
	uint16_t len;
	uint16_t head;
	uint16_t tail;
} gprs_send_buffer;

gprs_send_buffer gprs_log_buf;

void gprs_send_buf_init(gprs_send_buf);
void gprs_buf_push(log_entry, gprs_send_buf);



#endif /* GPRS_TRANSFER_PACKAGES_H_ */