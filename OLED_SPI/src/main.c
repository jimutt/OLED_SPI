#include <asf.h>
#include "menu_list.h"
#include "globals.h"
#include "menu_buttons.h"
#include "menus.h"
#include "views.h"

#define MAX_RX_BUFFER_LENGTH	1
#define COMMAND_BUFFER_SIZE		64
volatile uint8_t incoming_byte[MAX_RX_BUFFER_LENGTH];

struct usart_module SIM808_usart;

typedef struct {
	unsigned char command[COMMAND_BUFFER_SIZE];
	uint8_t position;
	uint8_t available;
} command_buffer;

command_buffer SIM808_buf;

void usart_read_callback(struct usart_module *const usart_module)
{
	if(incoming_byte[0] == '\n') {
		if(SIM808_buf.position > 1) {
			SIM808_buf.command[SIM808_buf.position] = '\0';
			SIM808_buf.position = 0;
			SIM808_buf.available = 1;
		}
	}
	else if(incoming_byte[0] != '\r'){
		SIM808_buf.command[SIM808_buf.position] = incoming_byte[0];
		SIM808_buf.position++;
	}
	
}

void usart_write_callback(struct usart_module *const usart_module)
{
	//Write callback
}

static void init_SIM808_uart(void) {
	struct usart_config uart_settings;
	usart_get_config_defaults(&uart_settings);
		
	//Set clock to 8M
	uart_settings.generator_source = GCLK_GENERATOR_0;
		
	uart_settings.mux_setting = USART_RX_1_TX_2_XCK_3;
	uart_settings.pinmux_pad0 = PINMUX_PA10C_SERCOM0_PAD2; // Tx
	uart_settings.pinmux_pad1 = PINMUX_PA09C_SERCOM0_PAD1; // Rx
	uart_settings.pinmux_pad2 = PINMUX_UNUSED;
	uart_settings.pinmux_pad3 = PINMUX_UNUSED;
	uart_settings.baudrate = 9600;
	while (usart_init(&SIM808_usart, SERCOM0, &uart_settings) != STATUS_OK){}
		
	stdio_serial_init(&SIM808_usart, SERCOM0, &uart_settings);
	usart_enable(&SIM808_usart);
}

void init_sim808_usart_callbacks(void)
{
	usart_register_callback(&SIM808_usart, usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&SIM808_usart, usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&SIM808_usart, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&SIM808_usart, USART_CALLBACK_BUFFER_RECEIVED);
}

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
		usart_read_buffer_job(&SIM808_usart, (uint8_t *)incoming_byte, MAX_RX_BUFFER_LENGTH);
		
		if(SIM808_buf.available == 1) {
			printf("New Command: %s\r\n", SIM808_buf.command);
			SIM808_buf.available = 0;
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
