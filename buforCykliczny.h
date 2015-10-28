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
	Typ buff[SZ] ;
	uint8_t i, j ;
	static const uint8_t MASKA = SZ-1 ;

	// i - wskaźnik do wstawiania
	// j - wskaźnik do wyjmowania

public :

	bool wstaw(Typ data)
	{
		if(!czyPelny())
		{
			buff[i] = data ;
			i = (i+1) & MASKA ;
			return true ;
		}

		return false ;
	}

	bool wyjmij(Typ &dest)
	{
		if(!czyPusty())
		{
			*dest = buff[j] ;
			j = (j+1) & MASKA ;
			return true ;
		}

		return false ;
	}
	bool czyPusty() const
	{
		// pusty jest, gdy oba wskaźniki wskazują na to samo miejsce
		// czyli, że wskażnik j cofnął się aż do
		return i == j ;
	}
	bool czyPelny() const
	{
		// nie możemy wstawić więcej, czyli i wskazuje na pozycję
		// jeden mniejszą niż j
		return ((j-1) & MASKA) == i ;
	}

	BuforCykliczny() : i(0), j(0) { }
};


#endif /* USART_BUFORCYKLICZNY_H_ */
