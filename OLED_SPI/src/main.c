#define BTN_MENU_SELECT		PIN_PA14
#define BTN_MENU_NAV_DOWN	PIN_PA15

#include <asf.h>
#include "menus.h"

struct port_config pin_cfg;

void display_menu(menu_link menu) {
	gfx_mono_prev_menu = gfx_mono_active_menu;
	gfx_mono_active_menu = menu;
	gfx_mono_menu_init(&menu_list[menu]);
	ssd1306_write_display();
}

void menu_buttons_init() {
	port_get_config_defaults(&pin_cfg);
	pin_cfg.direction = PORT_PIN_DIR_INPUT;
	pin_cfg.input_pull = PORT_PIN_PULL_UP;
	
	port_pin_set_config(BTN_MENU_NAV_DOWN, &pin_cfg);
	port_pin_set_config(BTN_MENU_SELECT, &pin_cfg);
}

int main (void)
{
	system_init();
	delay_init();
	
	gfx_mono_init();
	
	menu_buttons_init();

	//! the page address to write to
	uint8_t page_address = 0;
	//! the column address, or the X pixel.
	uint8_t column_address = 0;

	

	// Initialize SPI and SSD1306 controller
	ssd1306_init();

	// set addresses at beginning of display
	ssd1306_set_page_address(7);
	ssd1306_set_column_address(0);
	
	uint8_t num = GFX_MONO_MENU_KEYCODE_DOWN;
	uint8_t y = 0;

	ssd1306_clear_display();
	
	display_menu(MAIN_MENU);
	
	// scroll the display using hardware support in the LCD controller
	while (true) {
		if(!port_pin_get_input_level(BTN_MENU_NAV_DOWN)) {
			gfx_mono_menu_process_key(&menu_list[gfx_mono_active_menu], GFX_MONO_MENU_KEYCODE_DOWN);
			ssd1306_write_display();
		}
		if(!port_pin_get_input_level(BTN_MENU_SELECT)) {
			volatile uint8_t menuChoice = gfx_mono_menu_process_key(&menu_list[gfx_mono_active_menu], GFX_MONO_MENU_KEYCODE_ENTER);
			menu_link menu = menu_list[gfx_mono_active_menu].element_links[menuChoice];
			if(menu == EXIT_MENU) {
				menu = menu_list[gfx_mono_active_menu].parent;
			}
			display_menu(menu);
			ssd1306_write_display();
		}
	}
}
