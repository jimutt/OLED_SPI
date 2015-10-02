/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

int main (void)
{
	system_init();
	
	gfx_mono_init();
	
	const char *menuItems[5] = {"Val 1", "Val 2", "Afas", "ASFF", "Exit"};
	
	struct gfx_mono_menu super_menu;
	super_menu.title = "Menu";
	super_menu.strings = menuItems;
	super_menu.current_page = 2;
	super_menu.num_elements = 5;
	super_menu.current_selection = 1;

	//! the page address to write to
	uint8_t page_address = 0;
	//! the column address, or the X pixel.
	uint8_t column_address = 0;

	system_init();
	delay_init();

	// Initialize SPI and SSD1306 controller
	ssd1306_init();

	// set addresses at beginning of display
	ssd1306_set_page_address(7);
	ssd1306_set_column_address(0);
	
	uint8_t num = 0;
	uint8_t y = 0;

	ssd1306_clear_display();
	
	gfx_mono_menu_init(&super_menu);
	
	// scroll the display using hardware support in the LCD controller
	while (true) {
		//gfx_mono_draw_line(10, 10, 20, 20, GFX_PIXEL_SET);
		//gfx_mono_draw_filled_rect(50,20,30,20,GFX_PIXEL_SET);
		//gfx_mono_draw_circle(20,40,10,GFX_PIXEL_SET, GFX_WHOLE);
		ssd1306_write_display();
		gfx_mono_menu_process_key(&super_menu, GFX_MONO_MENU_KEYCODE_DOWN);
		//ssd1306_draw_huge_number(15,0, num);
		//ssd1306_set_pixel(num, y);
		//ssd1306_write_display();
		//ssd1306_set_coordinate(0, 0);
		//ssd1306_write_data(0xFF);
		//ssd1306_set_coordinate(0, 0);
		//ssd1306_write_data(0xFF);

		delay_ms(200);
		num++;
		if(num > 127) {
			 num = 0;
			 y++;
		}
		if(y > 64) {
			y = 0;
		}
	}
}
