/*


 * pcd8544.cpp
 * Sterownik obsługi wyświetlacza monochromatycznego
 *  Created on: 2 kwi 2015
 *      Author: dicker
 */
typedef unsigned char 	uint8_t ;
typedef unsigned int	uint16_t ;
typedef unsigned char	coord_t ;
typedef bool			color_t ;
/*
 * Wyprowadzenia wyświetlacza
 * 8-led
 * 7-sclk
 * 6-sdin(mosi in AVR)
 * 5-d/c
 * 4-rst
 * 3-sce
 * 2-gnd
 * 1-vcc
 */

#define LCD_CLK 	PB7
#define LCD_DATA	PB5
#define LCD_DC		PB3
#define LCD_CE		PB4
#define LCD_RST		PB2

// CLK
#define LCD_CLK_SET       PORTB |=  (1<<LCD_CLK)
#define LCD_CLK_CLR       PORTB &= ~(1<<LCD_CLK)
#define LCD_CLK_SET_OUT   DDRB  |=  (1<<LCD_CLK)
// DATA
#define LCD_DATA_SET      PORTB |=  (1<<LCD_DATA)
#define LCD_DATA_CLR      PORTB &= ~(1<<LCD_DATA)
#define LCD_DATA_SET_OUT  DDRB  |=  (1<<LCD_DATA)
// DC
#define LCD_DC_SET        PORTB |=  (1<<LCD_DC)
#define LCD_DC_CLR        PORTB &= ~(1<<LCD_DC)
#define LCD_DC_SET_OUT    DDRB  |=  (1<<LCD_DC)
// CE
#define LCD_CE_SET        PORTB |=  (1<<LCD_CE)
#define LCD_CE_CLR        PORTB &= ~(1<<LCD_CE)
#define LCD_CE_SET_OUT    DDRB  |=  (1<<LCD_CE)
// RST
#define LCD_RST_SET       PORTB |=  (1<<LCD_RST)
#define LCD_RST_CLR       PORTB &= ~(1<<LCD_RST)
#define LCD_RST_SET_OUT   DDRB  |=  (1<<LCD_RST)

class pcd8544_display
{

	//void (*wsk_set_curr_pixel)(uint8_t) ;	// wskaźnik do funkcji wysyłającej do wyświetlacza dane
	//void (*wsk_gotoxy)(coord_t x, coord_t y) ;	// wskaźnik do funkcji wysyłającej do wyświetlacza komendę
	//color_t (*wsk_get_pixel)(coord_t x, coord_t y) ;	// wskaźnik do funkcji odbierającej dane z wyświetlacza

	coord_t x, y; // aktualne położenie kursora

	color_t curr_color ;
	bool append_mode ;

	static const coord_t MAX_X = 84 ;
	static const coord_t MAX_Y = 48 ;

	uint8_t memory[MAX_Y/8][MAX_X] ; // wyświetlacza parametry to (6*8)=48pikseli na 84pikseli


	enum commands_e
	{
			cmd_extended_instruction = 0x21 ,
			cmd_bias_1_48 = 0x13 ,
			cmd_temperature_control = 0x06 ,
			cmd_set_vop = 0xa5 ,
			cmd_basic_instruction_horizontal = 0x20 ,
			cmd_normal_mode = 0x0C
	};

	void send_command(uint8_t cmd) ;
	void send_data(uint8_t data) ;
	void set_next_pixel() ;


public :

	pcd8544_display() ; // konstruktor = inicjalizacja wyświetlacza
	void reset_display() ;
	void duplicate_pixels(uint16_t n) ;	// powiela n pikseli od zadanego aktualnego położe


	void		set_color(color_t color) 	// ustawia kolor kursora
	{
		curr_color = color ;
	}

	color_t		get_color() 	// pobiera kolor kursora
	{
		return curr_color ;
 	}

	void set_append_mode(bool is_append_mode)
	{
		append_mode = is_append_mode ;
	}

	void 		set_pos(coord_t xx, coord_t yy) ; // ustawia aktualną pozycję kursora na dane współrzędne
	void 		set_curr_pixel() ;  // ustawia aktualny piksel, czyli na aktualnej pozycji kursora przypisuje aktualny kolor
	void 		set_pixel(coord_t xx, coord_t yy) ;	 // ustawia piksel o zadanych wspolrzednych
	color_t 	get_pixel(coord_t xx, coord_t yy) ; // pobiera kolor piksela o zadanych wspolrzednych

	void		clear() ;

	void draw_square(coord_t xx, coord_t yy, uint8_t size_s) ;
	void draw_square_filled(coord_t x, coord_t y, uint8_t size_s) ;
	void draw_line(coord_t x1, coord_t y1, coord_t x2, coord_t y2) ;
	void draw_circle(coord_t x_s, coord_t y_s, uint8_t r) ;
	void draw_circle_filled(coord_t x_s, coord_t y_s, uint8_t r) ;
	void draw_rectangle_filled(coord_t x1, coord_t y1, coord_t x2, coord_t y2) ;
	void putchar(coord_t x_s, coord_t y_s, char c, uint8_t font_scale) ;
	uint8_t print_text(const char* text, coord_t x, coord_t y, uint8_t f_scale) ;
	void put_clear_chars(coord_t x_s, coord_t y_s, uint8_t font_scale, uint8_t n) ;
};


