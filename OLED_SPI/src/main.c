#include <asf.h>
#include <string.h>
#include "menu_list.h"
#include "globals.h"
#include "menu_buttons.h"
#include "menus.h"
#include "views.h"
#include "response_actions.h"
#include "sim808_uart.h"
#include "gprs_transfer_packages.h"


int main (void)
{
	system_init();
	delay_init();
	
	//Init uart
	init_SIM808_uart();
	init_sim808_usart_callbacks();
	
	system_interrupt_enable_global();
	
	SIM808_buf.position = 0;
	SIM808_buf.available = 0;
	memset(SIM808_buf.command, 0, sizeof(unsigned char)*COMMAND_BUFFER_SIZE);
	
	CMD_RESET.cmd = "ATZ0";
	CMD_RESET.expected_response = "OK";
	CMD_RESET.callback_enabled = 0;
	
	CMD_NO_ECHO.cmd = "ATE0";
	CMD_NO_ECHO.expected_response = "OK";
	CMD_NO_ECHO.callback_enabled = 0;
	
	CMD_GPS_PWR_ON.cmd = "AT+CGPSPWR=1";
	CMD_GPS_PWR_ON.expected_response = "OK";
	CMD_GPS_PWR_ON.callback_enabled = 0;
	
	CMD_GPS_PWR_OFF.cmd = "AT+CGPSPWR=0";
	CMD_GPS_PWR_OFF.expected_response = "OK";
	CMD_GPS_PWR_OFF.callback_enabled = 0;
	
	CMD_GET_GPS_DATA.cmd = "AT+CGPSINF=32";
	CMD_GET_GPS_DATA.callback_enabled = 1;
	CMD_GET_GPS_DATA.expected_response = "+CGPSINF";
	CMD_GET_GPS_DATA.response_cb = &SIM808_response_gps_data;
	
	CMD_GET_GPS_FIX.cmd = "AT+CGPSSTATUS?";
	CMD_GET_GPS_FIX.callback_enabled = 0;
	CMD_GET_GPS_FIX.expected_response = "Location 3D";
	
	CMD_GPRS_GET_REQ.cmd = "AT+HTTPACTION=0";
	CMD_GPRS_GET_REQ.callback_enabled = 1;
	CMD_GPRS_GET_REQ.expected_response = "OK";
	CMD_GPRS_GET_REQ.response_cb = &SIM808_response_gprs_get;
	
	gfx_mono_init();
	
	menu_buttons_init();
	btn_timer_config();
	btn_timer_config_callbacks();
	
	device.speed = 22; //Only for debug

	// The page address to write to
	uint8_t page_address = 0;
	// The column address, or the X pixel.
	uint8_t column_address = 0;

	// Initialize SPI and SSD1306 controller
	ssd1306_init();

	ssd1306_clear_display();
	display_menu(MAIN_MENU);
	
	port_get_config_defaults(&pin_cfg);
	pin_cfg.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(SIM808_RESET_PIN, &pin_cfg);
	
	gfx_mono_draw_filled_rect(0, 0,
	GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT, GFX_PIXEL_CLR);
	gfx_mono_draw_string("Enabling",23, 18, &sysfont);
	gfx_mono_draw_string("GPRS",44, 32, &sysfont);
	ssd1306_write_display();
	sim808_init();
	
	gfx_mono_draw_filled_rect(0, 0,
	GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT, GFX_PIXEL_CLR);
	gfx_mono_draw_string("Waiting for",10, 18, &sysfont);
	gfx_mono_draw_string("GPS fix",30, 32, &sysfont);
	ssd1306_write_display();
	
	//TODO: Vänta på GPS location fix
	while (1) {
		sim808_send_command(CMD_GET_GPS_DATA);
		sim808_parse_response_wait();
		delay_ms(1100);
		sim808_send_command(CMD_GET_GPS_DATA);
		sim808_parse_response_wait();
		
		gprs_send_data_log();
		
		delay_ms(999000);
	}
	
	gfx_mono_draw_filled_rect(0, 0,
	GFX_MONO_LCD_WIDTH, GFX_MONO_LCD_HEIGHT, GFX_PIXEL_CLR);
	gfx_mono_draw_string("Yeah!",50, 18, &sysfont);
	ssd1306_write_display();
	
	//GPRS GET TEST
	sim808_send_command(CMD_GPRS_GET_REQ);
	sim808_parse_response_wait();
	sim808_parse_response_wait();
	delay_ms(200000);
	
	
	
	
	while (true) {
		
		sim808_send_command(CMD_GET_GPS_DATA);
		delay_ms(2000);
		
		if(SIM808_buf.available == 1) {
			sim808_parse_response();
		}
			
			
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
