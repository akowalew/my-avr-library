# LCD44780

Zbiór funkcji do obsługi wyświetlacza alfanumerycznego o typowym sterowniku HD44780

Całość napisana i przetestowana pod platformą AVR.

Niniejszy zbiór jest także sprawdzeniem możliwości kompilatora avr-g++, który obsługuje
nie, tak jak zwykle, język C, ale C++. 

Biblioteka zadziała z większością wyświetlaczy numerycznych, począwszy od 8x2 znaków, 16x1, czy też 16x2 znaków.
Do dopasowania się pod konkretny model wystarczy kilka modyfikacji w kodzie. 

Całość implementuje w sobie interfejs szeregowo-równoległy, bez oczekiwania na odpowiedź wyświetlacza.
