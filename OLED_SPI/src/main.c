#include <asf.h>
#include "menu_list.h"
#include "globals.h"
#include "menu_buttons.h"
#include "menus.h"

int main (void)
{
	system_init();
	delay_init();
	
	gfx_mono_init();
	
	menu_buttons_init();
	btn_timer_config();
	btn_timer_config_callbacks();

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
		
			if(btn_nav_down.active) {
				btn_nav_down.active = 0;
				gfx_mono_menu_process_key(&menu_list[gfx_mono_active_menu], GFX_MONO_MENU_KEYCODE_DOWN);
				ssd1306_write_display();
			}
			
			if(btn_nav_select.active) {
				btn_nav_select.active = 0;
				volatile uint8_t menuChoice = gfx_mono_menu_process_key(&menu_list[gfx_mono_active_menu], GFX_MONO_MENU_KEYCODE_ENTER);
				menu_link menu = menu_list[gfx_mono_active_menu].element_links[menuChoice];
				if(menu == EXIT_MENU) {
					menu_list[gfx_mono_active_menu].current_page = 0;
					menu_list[gfx_mono_active_menu].current_selection = 0;
					menu = menu_list[gfx_mono_active_menu].parent;
				}
				display_menu(menu);
				ssd1306_write_display();
			}

	}
}
