#include <asf.h>
#include "menu_list.h"
#include "globals.h"
#include "menu_buttons.h"
#include "menus.h"
#include "views.h"

int main (void)
{
	system_init();
	delay_init();
	
	gfx_mono_init();
	
	menu_buttons_init();
	btn_timer_config();
	btn_timer_config_callbacks();
	
	device.speed = 22; //Only for debug

	// the page address to write to
	uint8_t page_address = 0;
	// the column address, or the X pixel.
	uint8_t column_address = 0;

	// Initialize SPI and SSD1306 controller
	ssd1306_init();
	
	uint8_t num = GFX_MONO_MENU_KEYCODE_DOWN;
	uint8_t y = 0;

	ssd1306_clear_display();
	
	display_menu(MAIN_MENU);
	ssd1306_write_display();
	
	while (true) {
			
			
			if(btn_nav_down.active) {
				btn_nav_down.active = 0;
				gfx_mono_menu_process_key(&menu_list[gfx_mono_active_menu-(VIEW_MAX_INDEX+1)], GFX_MONO_MENU_KEYCODE_DOWN);
				ssd1306_write_display();
			}
			
			if(btn_nav_select.active) {
				btn_nav_select.active = 0;
				
				if(is_view(gfx_mono_active_menu)) {
					ssd1306_clear_display();
					display_menu(MAIN_MENU);
				}
				else {
					volatile uint8_t menuChoice = gfx_mono_menu_process_key(&menu_list[gfx_mono_active_menu-(VIEW_MAX_INDEX+1)], GFX_MONO_MENU_KEYCODE_ENTER);
					menu_link menu = menu_list[gfx_mono_active_menu-(VIEW_MAX_INDEX+1)].element_links[menuChoice];
									
					// TODO: Skriv om snyggare
					if(menu == EXIT_MENU) {
						menu_list[gfx_mono_active_menu-(VIEW_MAX_INDEX+1)].current_page = 0;
						menu_list[gfx_mono_active_menu-(VIEW_MAX_INDEX+1)].current_selection = 0;
										
										
						if(is_view(menu_list[gfx_mono_active_menu-(VIEW_MAX_INDEX+1)].parent)) {
							display_view(menu_list[gfx_mono_active_menu-(VIEW_MAX_INDEX+1)].parent);
						}
						else {
							menu = menu_list[gfx_mono_active_menu-(VIEW_MAX_INDEX+1)].parent;
							display_menu(menu);
						}
										
					}
					else {
						display_menu(menu);
					}
				}
				
			}
			
	}
}
