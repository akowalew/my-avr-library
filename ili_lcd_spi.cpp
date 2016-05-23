/*
 * lcd_spi.cpp
 *
 *  Created on: 3 mar 2015
 *      Author: dicker
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "ili_lcd_spi.h"

const ili_color_t ili_color_table[] PROGMEM =
{
    	ili_color(0, 0, 0), ili_color(127, 0, 0),
	ili_color(255, 0, 0), ili_color(0, 127, 0),
	ili_color(0, 255, 0),  ili_color(0, 0, 127),
	ili_color(0, 0, 255), ili_color(127, 127, 0),
	ili_color(255, 255, 0), ili_color(127, 0, 127),
	ili_color(255, 0, 255), ili_color(0, 127, 127),
	ili_color(0, 255, 255), ili_color(127,127, 127),
	ili_color(255,255, 255)
} ;

void ili9341_lcd::setup_mcu(uint8_t res, uint8_t mosi, uint8_t sck)
{
	/* Konfiguracja mikrokontrolera :
	 * CS, DC, RES, MOSI, SCK - WYJSCIE
	 * SPI2X, czyli lacznie Fosc / 2
	 * Master(przechwycenie wejscia MISO)
	 */
	ILI_DDR |= (1 << cs_pin) | (1 << dc_pin) | (1 << res) | (1 << mosi) | (1 << sck) ;
	SPCR |= (1 << SPE) | (1 << MSTR) ;
	SPSR |= (1 << SPI2X) ;

	 // Sekcja resetu
	ILI_PORT |= (1 << res) ; 	_delay_ms(10) ;
	ILI_PORT &= ~(1 << res) ;	_delay_ms(10) ;
	ILI_PORT |= (1 << res) ; 	_delay_ms(150) ;
}

ili9341_lcd::ili9341_lcd(uint8_t dc, uint8_t cs, uint8_t res, uint8_t mosi, uint8_t sck) : dc_pin(dc), cs_pin(cs)
{
	setup_mcu(res, mosi, sck) ;
	init_display() ; // zainicjuj odpowiednie rejestry sterownika
}

void ili9341_lcd::init_display(void)
{
    // exit standby section
    send_cmd(ILI_CMD_SLEEP_OUT) ;
    deselect_chip() ;
    _delay_ms(150) ;

    send_cmd(ILI_CMD_DISPLAY_ON) ;
    deselect_chip() ;

    // init controller registers section
    send_cmd(ILI_CMD_POWER_CONTROL_A) ;
    send_data(0x39) ;
    send_data(0x2C) ;
    send_data(0x00) ;
    send_data(0x34) ;
    send_data(0x02) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_POWER_CONTROL_B) ;
    send_data(0x00) ;
    send_data(0xAA) ;
    send_data(0xB0) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_PUMP_RATIO_CONTROL) ;
    send_data(0x30) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_POWER_CONTROL_1) ;
    send_data(0x25) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_POWER_CONTROL_2) ;
    send_data(0x11) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_VCOM_CONTROL_1) ;
    send_data(0x5C) ;
    send_data(0x4C) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_VCOM_CONTROL_2) ;
    send_data(0x94) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_DRIVER_TIMING_CONTROL_A) ;
    send_data(0x85) ;
    send_data(0x01) ;
    send_data(0x78) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_DRIVER_TIMING_CONTROL_B) ;
    send_data(0x00) ;
    send_data(0x00) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_COLMOD_PIXEL_FORMAT_SET) ;
    send_data(0x05) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    set_orientation(0) ;
    set_screen_limits(0, 0, ILI_DEFAULT_WIDTH, ILI_DEFAULT_HEIGHT) ;

    font_scale = 1 ;
    set_color(ILI_COLOR_BLACK) ;
}


uint8_t ili9341_lcd::read_byte(void) const
{ // odczytuje jeden bajt z ekranu. WAŻNE, wczesniej należy wydać polecenie ILI_CMD_READ_MEMORY
	send_byte(0x00) ; // dummy data
    wait_for_sync_done() ;
    return SPDR ;
}


void ili9341_lcd::send_cmd(uint8_t cmd) const
{   // ustawia sterownik i wpisuje do niego komende. Po tej operacji mozna
    // zaczac wpisywac do niego dane instrukcja send_data
    select_chip() ;
    command() ;

    SPDR = cmd ;
    wait_for_sync_done() ;

    data() ;
}

void ili9341_lcd::send_data(uint8_t data) const
{   // wysyla dane do ekranu. WAZNE: wczesniej musi zostac wyslana komenda,
    // by odpowiednio ustawic sterownik. Po wyslaniu wszystkich danych trzeba zwolnic
    // sterownik instrukcja deselect_chip()
    SPDR = data ;
    wait_for_sync_done() ;
}


void ili9341_lcd::set_pixel(ili_coord_t x, ili_coord_t y)
{
    gotoxy(x,y) ;
    send_cmd(ILI_CMD_MEMORY_WRITE) ;

    send_data(actual_color) ;
    send_data(actual_color >> 8) ;

    wait_for_sync_done() ;
    deselect_chip() ;
}

ili_color_t ili9341_lcd::get_pixel(ili_coord_t x, ili_coord_t y)
{   // odczytujemy kolor danego piksela z ekranu i zwracamy jego kolor (8-COLOR !!!)
    uint8_t r, g, b ;

    gotoxy(x,y) ;

    send_cmd(ILI_CMD_MEMORY_READ) ;
    r = read_byte() ; // pierwszy bajt pomijamy, dane zbędne

    r = read_byte() ;
    g = read_byte() ;
    b = read_byte() ;

    deselect_chip() ;

    return ili_color(r,g,b) ;
}

void ili9341_lcd::set_color(ili_color_e color)
{
    actual_color = pgm_read_word(&ili_color_table[color]) ;
}

void ili9341_lcd::set_orientation(uint8_t flag)
{  // ustawia orientacje ekranu. Mozna zrobic rotacje wzgledem X, Y, albo zamienic obie wspolrzedne
    uint8_t control = 0x48 ;

    const uint8_t ILI_FLIP_X = 1 ;
    const uint8_t ILI_FLIP_Y = 2 ;
    const uint8_t ILI_SWITCH_XY = 4 ;

    if(flag & ILI_FLIP_X)
        control &= ~(1 << 6) ;
    if(flag & ILI_FLIP_Y)
        control &= ~(1 << 7) ;
    if(flag & ILI_SWITCH_XY)
        control |= (1 << 5) ;

    send_cmd(ILI_CMD_MEMORY_ACCESS_CONTROL) ;
    send_data(control) ;
    wait_for_sync_done() ;
    deselect_chip() ;
}

void ili9341_lcd::gotoxy(ili_coord_t x, ili_coord_t y)
{
    send_cmd(ILI_CMD_COLUMN_ADDRESS_SET) ;
    send_data(x >> 8) ;
    send_data(x & 0xFF) ;
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_PAGE_ADDRESS_SET) ;
    send_data(y >> 8) ;
    send_data(y & 0xFF) ;
    wait_for_sync_done() ;
    deselect_chip() ;
}

void ili9341_lcd::set_screen_limits( ili_coord_t s_x, ili_coord_t s_y,
									 ili_coord_t e_x, ili_coord_t e_y)
{ // przepisuje informacje dotyczace poczatkowych i koncowych punktow ekranu
  // nastepnie wysyla je na ekran
    start_x = s_x ;
    start_y = s_y ;
    end_x = e_x ;
    end_y = e_y ;

    setup_draw_limits(1) ;
}

void ili9341_lcd::setup_draw_limits(uint8_t setup_end_limits)
{   // wysyla do sterownika ekranu informacje dotyczace poczatkowych i koncowych
    // punktow ekranu.

    send_cmd(ILI_CMD_COLUMN_ADDRESS_SET) ;
    send_data(start_x >> 8) ;
    send_data(start_x & 0xFF) ;
    if(setup_end_limits)
    {
        send_data(end_x >> 8) ;
        send_data(end_x & 0xFF) ;
    }
    wait_for_sync_done() ;
    deselect_chip() ;

    send_cmd(ILI_CMD_PAGE_ADDRESS_SET) ;
    send_data(start_y >> 8) ;
    send_data(start_y & 0xFF) ;
    if(setup_end_limits)
    {
        send_data(end_y >> 8) ;
        send_data(end_y & 0xFF) ;
    }
    wait_for_sync_done() ;
    deselect_chip() ;
}














