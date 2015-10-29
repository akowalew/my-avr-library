/*
 * buforCykliczny.h
 *
 *  Created on: 28 paź 2015
 *      Author: akowalew
 */

#ifndef USART_BUFORCYKLICZNY_H_
#define USART_BUFORCYKLICZNY_H_

typedef unsigned char uint8_t ;

template<typename Typ, uint8_t SZ> class BuforCykliczny
{ // SZ - koniecznie potęga dwójki!
	volatile Typ buff[SZ] ;
	volatile uint8_t i, j ;
	static const uint8_t MASKA = SZ-1 ;
	volatile Typ lastAdd ;

	volatile bool czyPusty, czyPelny ;

	// i - wskaźnik do wstawiania
	// j - wskaźnik do wyjmowania

public :

	bool push(Typ data) {
		if(!isFull()) {
			buff[i] = data ;
			i = (i+1) & MASKA ;
			czyPusty = false ;
			if( i == j )
				czyPelny = true ;

			lastAdd = data ;
			return true ;
		}
		return false ;
	}

	bool pop(volatile Typ *dest) {
		if(!isEmpty()) {
			*dest = buff[j] ;
			j = (j+1) & MASKA ;

			czyPelny = false ;
			if(i == j)
				czyPusty = true ;
			return true ;
		}

		return false ;
	}

	bool isFull() const {
		return czyPelny ;
	}
	bool isEmpty() const {
		return czyPusty ;
	}

	Typ seeLastAdded() const {
		return lastAdd ;
	}

	BuforCykliczny() : i(0), j(0) , czyPusty(true), czyPelny(false) { }

};


#endif /* USART_BUFORCYKLICZNY_H_ */
