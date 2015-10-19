#include "pcd8544.h"
#include <avr/io.h>
#include <util/delay.h>

#include <avr/pgmspace.h>

const uint8_t fonts[] PROGMEM =
{
#include "font6x8p.h"
};

inline coord_t abs(coord_t a)
{
	return (a>=0) ? a : (-a) ;
}

void pcd8544_display::set_pos(coord_t xx, coord_t yy)
{
	send_command(0x80 | (static_cast<uint8_t>(xx))) ;
	send_command(0x40 | (static_cast<uint8_t>(yy/8))) ;
	// setY : 01000YYY COMMAND
	// setX : 1XXXXXXX COMMAND
	x = xx ;
	y = yy ;
}

void pcd8544_display::set_next_pixel()
{
	if(++x == MAX_X)
	{
		x = 0 ;
		if(++y == MAX_Y)
			y = 0 ;
	}
	send_command(0x80 | (static_cast<uint8_t>(x))) ;
	send_command(0x40 | (static_cast<uint8_t>(y/8))) ;
}

void pcd8544_display::set_curr_pixel()
{
	if(curr_color == 0)
	{
		if(append_mode == true)
			memory[y / 8][x] &= ~(1 << (y % 8)) ;
		else
			memory[y / 8][x] = 0xFF & ~(1 << (y % 8)) ;

	}
	else
	{
		if(append_mode == true)
			memory[y / 8][x] |= (1 << (y % 8)) ;
		else
			memory[y / 8][x] = (1 << (y % 8)) ;
	}

	send_data(memory[y / 8][x]) ;	// adres w pamięci urządzenia się inkrementuje, my też musimy
}

void pcd8544_display::set_pixel(coord_t xx, coord_t yy)
{
	set_pos(xx,yy) ;
	set_curr_pixel() ;
}

color_t pcd8544_display::get_pixel(coord_t xx, coord_t yy)
{
	return ((memory[yy/8][xx]) & (1 << (yy % 8))) >> (yy % 8) ;
}

void pcd8544_display::send_command(uint8_t cmd)
{
    LCD_CE_CLR ;

    LCD_DC_CLR ;

    SPDR = cmd;
    while(!(SPSR & (1<<SPIF)));

    LCD_CE_SET ;
}

void pcd8544_display::send_data(uint8_t data)
{
    LCD_CE_CLR;

    LCD_DC_SET;

    SPDR = data;
    while(!(SPSR & (1<<SPIF)));

    LCD_CE_SET;
}

pcd8544_display::pcd8544_display() : x(0), y(0), curr_color(1), append_mode(true)
{
	LCD_RST_CLR;

    LCD_RST_SET_OUT ;
    LCD_CLK_SET_OUT ;
    LCD_DATA_SET_OUT ;
    LCD_DC_SET_OUT ;
    LCD_CE_SET_OUT ;

    // Konfiguracja sprzętowego SPI
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);

    reset_display() ;
}

void pcd8544_display::reset_display()
{
	 LCD_RST_CLR;
   //   < 30ms
	_delay_ms(15);

	LCD_RST_SET ;

	LCD_CE_SET ;

	send_command(0x21); // Function set - extended instruction set
	send_command(0x05); // Bias - 1:48
	send_command(0x14); // Temperature Control
	send_command(0xbf); // Set Vop
	send_command(0x20); // Function set - basic instruction set, horizontal addressing
	send_command(0x0c); // Display control - normal mode

	clear() ;
}

void pcd8544_display::clear()
{
    unsigned char  i,j;

    set_pos(0,0) ;

    for(i=0; i<MAX_Y/8; i++)
    {
        set_pos(0, i*8) ;
        for(j=0; j<MAX_X; j++)
        {
	       send_data(0x00);
	       memory[i][j] = 0 ;
        }
    }

    set_pos(0,0) ;
}

void pcd8544_display::duplicate_pixels(uint16_t n)
{ // powtarza n pikseli o tym samym kolorze
    while(n--)
    {
        set_curr_pixel() ;
        set_next_pixel() ;
    }
}


void pcd8544_display::draw_square(coord_t xx, coord_t yy, uint8_t size_s)
{ // x i y to wspolrzedne LEWEGO NAROZNIKA kwadratu ktory bedzie rysowany
    uint8_t i ;

    set_pos(xx, yy) ;
    duplicate_pixels(size_s) ; // pierwsza belka


    for( i = 1 ; i < size_s - 1 ; i++)
    {
        ++yy ;
        set_pixel(xx,yy) ;
        set_pixel(xx+size_s-1, yy) ;
    }

    if(size_s > 1)
    {
        set_pos(xx, ++yy) ;
        duplicate_pixels(size_s) ;
    }
}

void pcd8544_display::draw_square_filled(coord_t x, coord_t y, uint8_t size_s)
{ // x i y to wspolrzedne LEWEGO NAROZNIKA kwadratu ktory bedzie rysowany
    uint8_t i ;

    for(i = 0 ; i < size_s ; i++)
    {
        set_pos(x,y++) ;
        duplicate_pixels(size_s) ;
    }
}

void pcd8544_display::draw_line(coord_t x1, coord_t y1, coord_t x2, coord_t y2)
{ // rysuje linie od punktu do punktu o kolorze zadanym
	coord_t dx = x2 - x1,       // roznica iksow
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

void pcd8544_display::draw_circle(coord_t x_s, coord_t y_s, uint8_t r)
{
    int16_t e, e1, e2 ;
    coord_t x, y ;

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

void pcd8544_display::draw_circle_filled(coord_t x_s, coord_t y_s, uint8_t r)
{
    int16_t e, e1, e2 ;
    coord_t x, y ;

    x = 0 ; y = r ; e = 0 ;
    // poczatkowe wspolrzedne punktu startowego i wyrazenie bledu

    while(x <= y)
    {
        set_pos(-y+x_s, x + y_s) ;
        duplicate_pixels(y<<1) ; // linia wew gorna

        set_pos(-y+x_s, -x + y_s) ;
        duplicate_pixels(y<<1) ; // linia wew dolna
        e1 = e + (x << 1) + 1 ; // blad dla P1
        e2 = e1 - (y << 1) + 1 ; // blad dla P2

        if(e1 + e2 >= 0)
        {
            set_pos(-x+x_s, y + y_s) ;
            duplicate_pixels(x<<1) ; // zew gorna
            set_pos(-x+x_s, -y + y_s) ;
            duplicate_pixels(x<<1) ; // zew dolna
            y-- ; // wbieramy punkt P2
            e = e2 ;
        }
        else
            e = e1 ;

        x++ ;
    }
}

void pcd8544_display::putchar(coord_t x_s, coord_t y_s, char c, uint8_t font_scale)
{ // horizontal adressing
    uint8_t data ;
    coord_t x, y ;
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

uint8_t pcd8544_display::print_text(const char* text, coord_t x, coord_t y, uint8_t f_scale)
{ // funkcja zwraca ilość wpisanych znakow, pomocne np. przy okreslaniu dlugosci zapisanego napisu
    uint8_t ret = 0 ;
    char c ;
    uint8_t xx = x ;

    while((c = *(text++)))
    {
        ret++ ;
        putchar(x, y, c, f_scale) ;
        x += 6*f_scale ;
        if(x + 6*f_scale >= MAX_X)
        {
        	x = xx ;
        	y += 8*f_scale ;

        	if(y + 8*f_scale >= MAX_Y)
        		return ret ;
        }
    }
    return ret ;
}

void pcd8544_display::draw_rectangle_filled(coord_t x1, coord_t y1, coord_t x2, coord_t y2)
{   // rusuje prostokat o lewy skrajnym wierzcholku (x1,y1) i prawym skrajnym dolnym wierzcholku (x2,y2)
	coord_t n = x2 - x1 + 1 ;
	coord_t y = y1 ;

    for(y = y1 ; y <= y2 ; y++)
    {
        set_pos(x1,y) ;
        duplicate_pixels(n) ;
    }
}

void pcd8544_display::put_clear_chars(coord_t x_s, coord_t y_s, uint8_t font_scale, uint8_t n)
{

	// is it right coded ?
	set_append_mode(false) ;
	for(uint8_t i = 0 ; i < n ; i++)
	{
		putchar(x_s, y_s, ' ', font_scale) ;
		x_s += 6*font_scale ;
	}
	set_append_mode(true) ;
}

