/*
 * ili_lcd_spi_graph.cpp
 *
 *  Created on: 3 mar 2015
 *      Author: dicker
 */
// ILI_LCD_SPI_GRAPH.C
// w pliku tym zawarłem definicje funkcji które są potrzebne stricte do tworzenia grafiki na wyświetlaczu
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ili_lcd_spi.h"


const uint8_t fonts[] PROGMEM =
{
    #include "font6x8p.h"
} ;

inline ili_coord_t abs(ili_coord_t a)
{
	return (a>=0) ? a : (-a) ;
}


void ili9341_lcd::duplicate_pixels(uint32_t n)
{ // powtarza n pikseli o tym samym kolorze
    send_cmd(ILI_CMD_MEMORY_WRITE) ;

    while(n--)
    {
        send_data(actual_color) ;
        send_data(actual_color >> 8) ;
    }

    wait_for_sync_done() ;
    deselect_chip() ;
}


void ili9341_lcd::clear_screen(void)
{ // wypelnia caly ekran jednym kolorem
    set_screen_limits(0,0, ILI_DEFAULT_WIDTH, ILI_DEFAULT_HEIGHT) ;

    duplicate_pixels(ILI_TOTAL_PIXELS) ;
}

void ili9341_lcd::draw_square(ili_coord_t x, ili_coord_t y, uint8_t size_s)
{ // x i y to wspolrzedne LEWEGO NAROZNIKA kwadratu ktory bedzie rysowany
    uint8_t i ;

    gotoxy(x, y) ;
    duplicate_pixels(size_s) ; // pierwsza belka


    for( i = 1 ; i < size_s - 1 ; i++)
    {
        ++y ;
        set_pixel(x,y) ;
        set_pixel(x+size_s-1, y) ;
    }

    if(size_s > 1)
    {
        gotoxy(x, ++y) ;
        duplicate_pixels(size_s) ;
    }
}

void ili9341_lcd::draw_square_filled(ili_coord_t x, ili_coord_t y, uint8_t size_s)
{ // x i y to wspolrzedne LEWEGO NAROZNIKA kwadratu ktory bedzie rysowany
    uint8_t i ;

    for(i = 0 ; i < size_s ; i++)
    {
        gotoxy(x,y++) ;
        duplicate_pixels(size_s) ;
    }
}

void ili9341_lcd::draw_line(ili_coord_t x1, ili_coord_t y1, ili_coord_t x2, ili_coord_t y2)
{ // rysuje linie od punktu do punktu o kolorze zadanym
    ili_coord_t dx = x2 - x1,       // roznica iksow
                dy = y2 - y1, k ;   // rocniza igrekow, licznik
    int16_t e = dx / 2 ;     // blad oszacowania, krok x, krok y
    int8_t kx, ky ;

    kx = (x1 <= x2) ? 1 : -1 ; // okreslenie kroku wzdluz X, czy w prawo czy w lewo
    ky = (y1 <= y2) ? 1 : -1 ; // okreslenie kroku wzdluz Y, czy w gore, czy w dol

    dx = abs(x2-x1) ; // odleglosc miedzy x1 i x2
    dy = abs(y2-y1) ; // miedzy y1 i y2

    set_pixel(x1, y1) ;

    if(dx < dy)
    {
        // WERSJA DLA KATOW WIEKSZYCH OD 45^
        e = dy / 2 ;
        for(k = 0 ; k < dy ; k++) // Y jest kierunkiem szybkim, wiec wykonujemy tylko dx razy to
        {
            y1 += ky ; // przesuniecie w kierunku szybkim
            e -= dx ; // kalkulacja wyrazenia bledu

            if(e < 0)
            { // wyrazenie bledu ujemne, czyli musimy wykonac ruch w kierunku wolnym
                x1 += kx ; // ruch w kierunku wolnym
                e += dy ; // obliczamy nowe wyrazenie bledu
            }

            set_pixel(x1,y1) ;
        }
    }
    else
    {
        // WERSJA Z NORMALNYMI WSPOLRZEDNYMI
        e = dx / 2 ;
        for(k = 0 ; k < dx ; k++) // x jest kierunkiem szybkim, wiec wykonujemy tylko dx razy to
        {
            x1 += kx ; // przesuniecie w kierunku szybkim
            e -= dy ; // kalkulacja wyrazenia bledu

            if(e < 0)
            { // wyrazenie bledu ujemne, czyli musimy wykonac ruch w kierunku wolnym
                y1 += ky ; // ruch w kierunku wolnym
                e += dx ; // obliczamy nowe wyrazenie bledu
            }

            set_pixel(x1,y1) ;
        }
    }
}

void ili9341_lcd::draw_circle(ili_coord_t x_s, ili_coord_t y_s, uint8_t r)
{
    int16_t e, e1, e2 ;
    ili_coord_t x, y ;

    x = 0 ;
    y = r ;
    e = 0 ; // poczatkowe wyrazenie bledu

    while(x <= y)
    {
        // zapalamy 8 pikseli, bo koło jest symetryczne wzgledem 4 prostych
        set_pixel(x+x_s, y+y_s) ;
        set_pixel(y+x_s, -x+y_s) ;
        set_pixel(-x + x_s, -y + y_s) ;
        set_pixel(-y+x_s, x + y_s) ;
        set_pixel(y+x_s, x+y_s) ;
        set_pixel(x+x_s, -y+y_s) ;
        set_pixel(-y+x_s, -x+y_s) ;
        set_pixel(-x+x_s, y+y_s) ;

        e1 = e + (x << 1) + 1 ; // wyrażenie błędu dla P1
        e2 = e1 - (y << 1) + 1 ; // wyrażenie błędu dla P2
        x++ ; //  Iks zawsze zwiekszamy o jeden
        if(e1+e2 >= 0)
        {
            y-- ; // suma jest nieujemna, czyli punkt P2 jest blizej okregu
            e = e2 ; // wybralismy punkt P2 wiec takze wybieramy jego 'e'
        }
        else
            e = e1 ; // wybralismy punkt P1, czyli wybieramy jego 'e'
    }
}

void ili9341_lcd::draw_circle_filled(ili_coord_t x_s, ili_coord_t y_s, uint8_t r)
{
    int16_t e, e1, e2 ;
    ili_coord_t x, y ;

    x = 0 ; y = r ; e = 0 ;
    // poczatkowe wspolrzedne punktu startowego i wyrazenie bledu

    while(x <= y)
    {
        gotoxy(-y+x_s, x + y_s) ;
        duplicate_pixels(y<<1) ; // linia wew gorna

        gotoxy(-y+x_s, -x + y_s) ;
        duplicate_pixels(y<<1) ; // linia wew dolna
        e1 = e + (x << 1) + 1 ; // blad dla P1
        e2 = e1 - (y << 1) + 1 ; // blad dla P2

        if(e1 + e2 >= 0)
        {
            gotoxy(-x+x_s, y + y_s) ;
            duplicate_pixels(x<<1) ; // zew gorna
            gotoxy(-x+x_s, -y + y_s) ;
            duplicate_pixels(x<<1) ; // zew dolna
            y-- ; // wbieramy punkt P2
            e = e2 ;
        }
        else
            e = e1 ;

        x++ ;
    }
}


void ili9341_lcd::putchar(ili_coord_t x_s, ili_coord_t y_s, char c)
{ // horizontal adressing
    uint8_t data ;
    ili_coord_t x, y ;
    const uint8_t * ptr = &fonts[(c-32)*6] ; // wskaznik na stałą

    for(x = 0 ; x < 6 ; x++)
    {
        data = pgm_read_byte(ptr++) ; // szybsza operacja, przesuwamy wskaznik
        for(y = 0 ; y < 8 ;y++)
        {
            if(data & (1 << y)) // jesli mamy tusz na danym pikselu
                draw_square_filled(x*font_scale + x_s, y*font_scale + y_s, font_scale) ; // wyskalowany i przesuniety o wektor [x_s,y_s]
        }
    }
}

uint8_t ili9341_lcd::print_text(char* text, ili_coord_t x, ili_coord_t y, uint8_t f_scale)
{ // funkcja zwraca ilość wpisanych znakow, pomocne np. przy okreslaniu dlugosci zapisanego napisu
    uint8_t ret = 0 ;
    font_scale = f_scale ;
    char c ;

    while((c = *(text++)))
    {
        ret++ ;
        putchar(x, y, c) ;
        x += 6*font_scale ;
        if(x + 6*font_scale >= ILI_DEFAULT_WIDTH)
            return ret ;
    }
    return ret ;
}

void ili9341_lcd::draw_rectangle_filled(ili_coord_t x1, ili_coord_t y1, ili_coord_t x2, ili_coord_t y2)
{   // rusuje prostokat o lewy skrajnym wierzcholku (x1,y1) i prawym skrajnym dolnym wierzcholku (x2,y2)
    ili_coord_t n = x2 - x1 + 1 ;
    ili_coord_t y = y1 ;

    for(y = y1 ; y <= y2 ; y++)
    {
        gotoxy(x1,y) ;
        duplicate_pixels(n) ;
    }
}



