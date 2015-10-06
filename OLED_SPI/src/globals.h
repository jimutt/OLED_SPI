/*
 * globals.h
 *
 * Created: 2015-10-05 14:05:59
 *  Author: jiut0001
 */ 


#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <asf.h>

struct port_config pin_cfg;
struct tc_module btn_timer;

typedef struct {
	uint8_t active;
	uint8_t pin;
	uint8_t lastState;
} button;

typedef struct {
	uint8_t speed;
	uint8_t lat;
	uint8_t lng;
	uint8_t height;
} position_info;

position_info device;


#endif /* GLOBALS_H_ */