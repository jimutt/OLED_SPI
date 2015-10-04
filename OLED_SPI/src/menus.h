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

menu_link gfx_mono_active_menu;
menu_link gfx_mono_prev_menu;

const char *main_menu_items[] = {"GSM", "Display", "Exit"};
const char *gsm_menu_items[] = {"Module", "Idle mode", "Logging", "Back"};
const char *gsm_module_menu_items[] = {"Turn off", "Back"};
const char *gsm_logging_menu_items[] = {"10 sec", "30 sec", "1 min", "5 min", "10 min", "30 min", "1 hour", "Back"};
const char *gsm_idle_menu_items[] = {"Back"};
const char *display_menu_items[] = {"Sleep mode", "Power", "Back"};

struct gfx_mono_menu menu_list[]= {
	// { Title, menu items, number of items, links for the items, parent menu, current selection, current position }
	// Main menu.
	{"Settings" , main_menu_items, 3, (menu_link[]){GSM_MENU, DISPLAY_MENU, EXIT_MENU}, MAIN_MENU, 0, 0},
	
	// GSM main menu.
	{"GSM" , gsm_menu_items, 4, (menu_link[]){GSM_MODULE_MENU, GSM_IDLE_MENU, GSM_LOGGING_MENU, EXIT_MENU}, MAIN_MENU, 0, 0},
		
	// GSM power settings.
	{"GSM Module" , gsm_module_menu_items, 2, (menu_link[]){NO_EVENT, EXIT_MENU}, GSM_MENU, 0, 0},
	
	// GSM logging frequency.
	{"Logging freq.", gsm_logging_menu_items, 8, (menu_link[]){EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU}, GSM_MENU, 0, 0},
	
	// Idle mode menu.
	{"Idle Mode", gsm_idle_menu_items, 1, (menu_link[]){EXIT_MENU}, GSM_MENU, 0, 0},
		
	// Display menu
	{"Display", display_menu_items, 3, (menu_link[]){EXIT_MENU, EXIT_MENU, EXIT_MENU}, MAIN_MENU, 0, 0} //Fix sub menus
};

/*
	{"GSM" , gsm_menu_items, 4, {GSM_MODULE_MENU, GSM_IDLE_MENU, GSM_LOGGING_MENU, EXIT_MENU}, 0, 0},
	{"Logging freq.", gsm_logging_menu_items, 8, {EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU,EXIT_MENU}, 0, 0},
	{"Idle Mode", gsm_idle_menu_items, 1,{EXIT_MENU}, 0, 0}
		*/




#endif /* MENUS_H_ */