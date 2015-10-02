/*
 * menus.h
 *
 * Created: 2015-10-02 17:35:31
 *  Author: Jimmy
 */ 


#ifndef MENUS_H_
#define MENUS_H_

#include "asf.h"
#include "menu_link.h"

menu_link gfx_active_menu;
menu_link gfx_prev_menu;

const char *main_menu_items[] = {"GSM", "Display", "Exit"};
const char *gsm_menu_items[] = {"Module", "Idle mode", "Logging", "Back"};
const char *gsm_module_menu_items[] = {"Turn off", "Back"};
const char *gsm_logging_menu_items[] = {"10 sec", "30 sec", "1 min", "5 min", "10 min", "30 min", "1 hour", "Back"};
const char *gsm_idle_menu_items[] = {"Back"};
	
menu_link main_menu_item_links[] = {GSM_MENU, DISPLAY_MENU, EXIT_MENU};

struct gfx_mono_menu menu_list[]= {
	{"Settings" , main_menu_items, 3,{GSM_MENU, DISPLAY_MENU, EXIT_MENU}, 0, 0},
	{"GSM" , gsm_menu_items, 4, {GSM_MODULE_MENU, GSM_IDLE_MENU, GSM_LOGGING_MENU, EXIT_MENU}, 0, 0},
	{"Logging freq.", gsm_logging_menu_items, 8, {EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU}, 0, 0},
	{"Idle Mode", gsm_idle_menu_items, 1,{EXIT_MENU}, 0, 0}
};

/*
	{"GSM" , gsm_menu_items, 4, {GSM_MODULE_MENU, GSM_IDLE_MENU, GSM_LOGGING_MENU, EXIT_MENU}, 0, 0},
	{"Logging freq.", gsm_logging_menu_items, 8, {EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU}, 0, 0},
	{"Idle Mode", gsm_idle_menu_items, 1,{EXIT_MENU}, 0, 0}
		*/




#endif /* MENUS_H_ */