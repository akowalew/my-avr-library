# PCD8544

Biblioteka do obsługi wyświetlaczy graficznych o sterowniku typu PCD8544.
Całość napisania wyłącznie do wykorzystania przez procesory serii ATmega.

Wyświetlacze te można spotkać w starszych modelach telefonów Nokia (3310 itp). 
Oferują one binarną matrycę o rozdzielczości 96x48 pikseli oraz zestaw funkcji 
do obsługi sterownika wyświetlacza.

Niniejsze funkcje są rozszerzeniem do wyświetlacza. Oferują wstawianie pikseli,
czyszczenie ekranu oraz rysowanie podstawowych kształtów : linie, kwadraty etc. 

Ze względu na niewielkie rozmiary ekranu oraz jego binarną naturę pamięć wyświetlacza
jest zaprogramowana programowo na mikrokontrolerze, jako seria bajtów. Wyświetlacz 
nie daje możliwości odczytywania z jego pamięci.

Komunikacja odbywa się poprzez interfejs SPI wbudowany w mikrokontroler.

Aby skorzystać z biblioteki, wystarczy zmodyfikować kilka wpisów w konfiguracji, to znaczy 
konfigurację SPI, konfigurację IO oraz dopasowanie zegara systemowego.
