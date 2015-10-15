/*
 * gps_utils.c
 *
 * Created: 2015-10-15 15:04:36
 *  Author: jiut0001
 */ 
#include "gps_utils.h"

//Convert from ddmm.mmmm to decimal coordinates
float gps_utils_coord_to_dec(char* val) {
	float o;

	char minutes[8];
	
	char *dotPointer;
	dotPointer = strchr(val, '.');
	
	char degrees[dotPointer - val];
	
	strncpy(minutes, dotPointer-2, 7);
	strncpy(degrees, val, (dotPointer - val - 2));
	
	o = atof(degrees) + (atof(minutes)/60.0);
	
	return o;
}