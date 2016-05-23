# ILI9341

Biblioteka graficzna obsługująca kolorowy wyświetlacz ze sterownikiem ILI9341, 
o rozdzielczości 240x320 pikseli. Całość napisana pod procesory serii ATmega.
Typowy produkt, który można znaleźć w postaci modułu
do kupienia na portalach aukcyjnych.

Biblioteka wyposażona jest w algorytmy do generowania podstawowych kształtów graficznych 
oraz umieszczania skalowalnej czcionki, znanej z wyświetlaczy alfanumerycznych.

Oprócz tego, mamy do dyspozycji funkcje ustalające współrzędne prostokąta, na którym generujemy grafikę
wstawianie pojedynczych pikseli, wymazywanie ekranu.

Interfejs zapewnia komunikację w obie strony, DO wyświetlacza i OD wyświetlacza, przy pomocy
modułu SPI na pokładzie procesora. 

Ze względu na duże zapotrzebowanie na pamięć w celu obsłużenia wyświetlacza, wszystko
umieszczane jest na tamtejszej pamięci. Tak więc, by sprawdzić poszczególne piksele na ekranie, 
trzeba niestety odwoływać się do pamięci ekranu.


