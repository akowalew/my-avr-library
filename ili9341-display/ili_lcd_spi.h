/*
 * ili_lcd_spi.h
 *
 *  Created on: 3 mar 2015
 *      Author: dicker
 */

#ifndef ILI_LCD_SPI_H_
#define ILI_LCD_SPI_H_

// typ wspolrzednych kartezjanskich ekranu
typedef int16_t ili_coord_t ;
typedef uint16_t ili_color_t ;

enum ili_color_e
{
    ILI_COLOR_BLACK = 0,
    ILI_COLOR_RED_D,
    ILI_COLOR_RED,
    ILI_COLOR_GREEN_D,
    ILI_COLOR_GREEN, //
    ILI_COLOR_BLUE_D,
    ILI_COLOR_BLUE,
    ILI_COLOR_YELLOW_D,
    ILI_COLOR_YELLOW,
    ILI_COLOR_PINK_D,
    ILI_COLOR_PINK,
    ILI_COLOR_CYAN_D,
    ILI_COLOR_CYAN,
    ILI_COLOR_WHITE_D,
    ILI_COLOR_WHITE
}  ;
const 	uint8_t 	ILI_NUMBER_OF_COLORS = 	15 ;

// makrodefinicje, ktorych nie mozna rozwinac w funkcje inline, poniewaz sa to niskopoziomowe instrukcje mikrokontrolera
#define swap_16(u16) ((uint16_t) ((uint16_t)(u16) >> 8) | ((uint16_t)(u16) << 8))
#define ili_color(r,g,b) static_cast<ili_color_t>(swap_16(((static_cast<uint16_t>(b)) >> 3) | (((static_cast<uint16_t>(g)) << 3) & 0x07E0) | (((static_cast<uint16_t>(r)) << 8) & 0xF800)))
#define PGM_GETSTR( str, idx ) (char*)pgm_read_word( &str[ idx ] ) // funkcja do odczytu wskzanikow do napisow
#define ILI_DDR		DDRB
#define ILI_PORT	PORTB

class ili9341_lcd
{
private :
	ili_coord_t start_x ;
	ili_coord_t start_y ;
	ili_coord_t end_x ;
	ili_coord_t end_y;
	uint8_t font_scale ;
	ili_color_t actual_color ;
	const uint8_t dc_pin, cs_pin	;

	// ***** funkcje niskopoziomowe do obsługi portów	***************
	 void select_chip()	const	{ ILI_PORT &= ~(1 << cs_pin) ; }
	 void deselect_chip() const	{ ILI_PORT |= (1 << cs_pin) ; }
	 void command()	const		{ ILI_PORT &= ~(1 << dc_pin) ; }
	 void data() const			{ ILI_PORT |= (1 << dc_pin) ; }
	 void wait_for_sync_done() const	{	while(!(SPSR & (1 << SPIF))); }
	 void send_byte(uint8_t bt) const { SPDR = bt ; }
	 uint8_t get_byte() const			{ return SPDR ; }
	 void setup_mcu(uint8_t res, uint8_t mosi, uint8_t sck) ;
	// ****************************************************************

	uint8_t read_byte() const ;	// odczyt bajtu po SPI
	void send_cmd(uint8_t command) const ; //	wyslanie komendy po SPI
	void send_data(uint8_t data) const ; // wyslanie danych

	void setup_draw_limits(uint8_t setup_end_limits) ; // wysylanie do lcd wierzcholkow, limitow tak zwanych
	void putchar(ili_coord_t x_s, ili_coord_t y_s, char c) ; // część funkcji print_text.

	void init_display() ;
public :
	ili9341_lcd(uint8_t dc, uint8_t cs, uint8_t res, uint8_t mosi, uint8_t sck) ;	// kosntruktor z argumentami


	void set_orientation(uint8_t flag) ; // ustawienie orientacji ekranu
	void set_color(ili_color_e color) ;	// ustawienie aktualnego koloru
	void set_screen_limits( ili_coord_t s_x, ili_coord_t s_y,
		                    ili_coord_t e_x, ili_coord_t e_y) ;
	void 		clear_screen(void) ;	// wypelnienie ekranu aktualnym kolorem

	uint8_t 	print_text(char* text, ili_coord_t x, ili_coord_t y, uint8_t f_scale) ;

	void 		gotoxy(ili_coord_t x, ili_coord_t y) ;
	void 		set_pixel(ili_coord_t x, ili_coord_t y) ;
	ili_color_t get_pixel(ili_coord_t x, ili_coord_t y) ;

	void duplicate_pixels(uint32_t n) ;	// wypisanie n takich samych pikseli, poczawszy od aktualnego miejsca
	void draw_square(ili_coord_t x, ili_coord_t y, uint8_t size_s) ;
	void draw_square_filled(ili_coord_t x, ili_coord_t y, uint8_t size_s) ;
	void draw_line(ili_coord_t x1, ili_coord_t y1, ili_coord_t x2, ili_coord_t y2) ;
	void draw_circle(ili_coord_t x_s, ili_coord_t y_s, uint8_t r) ;
	void draw_rectangle_filled(ili_coord_t x1, ili_coord_t y1, ili_coord_t x2, ili_coord_t y2) ;
	void draw_circle_filled(ili_coord_t x_s, ili_coord_t y_s, uint8_t r) ;
};

const uint8_t ILI_CMD_SLEEP_OUT  		= 0x11;
const uint8_t ILI_CMD_DISPLAY_ON  		= 0x29;
const uint8_t ILI_CMD_POWER_CONTROL_A 		= 0xCD;
const uint8_t ILI_CMD_POWER_CONTROL_B 		= 0xCF;
const uint8_t ILI_CMD_PUMP_RATIO_CONTROL	= 0xF7;
const uint8_t ILI_CMD_POWER_CONTROL_1 		= 0xC0;
const uint8_t ILI_CMD_POWER_CONTROL_2 		= 0xC1;
const uint8_t ILI_CMD_VCOM_CONTROL_1  		= 0xC5;
const uint8_t ILI_CMD_VCOM_CONTROL_2  		= 0xC7;
const uint8_t ILI_CMD_DRIVER_TIMING_CONTROL_A =	0xE8;
const uint8_t ILI_CMD_DRIVER_TIMING_CONTROL_B =	0xEA;
const uint8_t ILI_CMD_COLMOD_PIXEL_FORMAT_SET =	0x3A;
const uint8_t ILI_CMD_MEMORY_ACCESS_CONTROL	= 0x36;
const uint8_t ILI_CMD_COLUMN_ADDRESS_SET  	= 0x2A;
const uint8_t ILI_CMD_PAGE_ADDRESS_SET    	= 0x2B;
const uint8_t ILI_CMD_MEMORY_WRITE    		= 0x2C;
const uint8_t ILI_CMD_MEMORY_READ     		= 0x2E;

const ili_coord_t ILI_DEFAULT_WIDTH  = 240 ;
const ili_coord_t ILI_DEFAULT_HEIGHT = 320 ;
const uint32_t ILI_TOTAL_PIXELS	 = (uint32_t)ILI_DEFAULT_HEIGHT*ILI_DEFAULT_WIDTH ;

const uint8_t  ILI_FONT_WIDTH  = 6 ;
const uint8_t  ILI_FONT_HEIGHT = 8 ;
const unsigned char ILI_ERASE_CHAR = 146 ;

#endif /* ILI_LCD_SPI_H_ */
