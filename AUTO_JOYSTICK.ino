#define P0 0  // BROWN_PIN
#define P1 1  // RED_PIN
#define P2 2  // BLACK_PIN
#define P3 3  // YELLOW_PIN
#define P4 4  // GREEN_PIN
#define P5 5  // BLUE_PIN

byte OUT = 0;
byte VALCODER = 0;
byte VALCODER_OLD = 0;

void setup() {

DDRD = 255;
DDRD &= ~(1 << P3);  // Выводы на вход (уст. 0)
DDRD &= ~(1 << P4);
DDRD &= ~(1 << P5);

DDRD &= ~(1 << P2);

PORTD = 0;
PORTD |= (1 << P3); // Подтягивающие резисторы для них
PORTD |= (1 << P4);
PORTD |= (1 << P5);

DDRB = 0;  // Пока все на вход
PORTB = 0;
}

void loop() {

while (1) {
  OUT = 0;
  delay (5);
  
  // Проверяем первый ряд кнопок
  PORTD |= (1 << P0); PORTD |= (1 << P1); PORTD |= (1 << P2); // Все 3 выхода = 1
  PORTD &= ~(1 << P0); // Устанавливаем 0 на нужном выходе
  delay (10); // После паузы считываем состояние кнопок
  if( !(PIND & (1 << P3)) ) { OUT = 1; } // Нажата кнопка
  if( !(PIND & (1 << P4)) ) { OUT = 2; } // Нажата кнопка
  if( !(PIND & (1 << P5)) ) { OUT = 3; } // Нажата кнопка

  // Проверяем второй ряд кнопок
  PORTD |= (1 << P0); PORTD |= (1 << P1); PORTD |= (1 << P2); // Все 3 выхода = 1
  PORTD &= ~(1 << P1); // Устанавливаем 0 на нужном выходе
  delay (10); // После паузы считываем состояние кнопок
  if( !(PIND & (1 << P3)) ) { OUT = 4; } // Нажата кнопка
  if( !(PIND & (1 << P4)) ) { OUT = 5; } // Нажата кнопка
  if( !(PIND & (1 << P5)) ) { OUT = 6; } // Нажата кнопка

  // Проверяем ряд валкодера
  PORTD |= (1 << P0); PORTD |= (1 << P1); PORTD |= (1 << P2); // Все 3 выхода = 1
  DDRD |= (1 << P2); // Порт - на выход
  PORTD &= ~(1 << P2); // Устанавливаем 0 на нужном выходе
  delay (10); // После паузы считываем состояние кнопок
  if( !(PIND & (1 << P3)) ) { VALCODER = 1; } // Нажата кнопка
  if( !(PIND & (1 << P4)) ) { VALCODER = 2; } // Нажата кнопка
  if( !(PIND & (1 << P5)) ) { VALCODER = 3; } // Нажата кнопка
  DDRD &= ~(1 << P2); // Порт - на вход

  if (VALCODER != VALCODER_OLD) { // Обработка валкодера
    if (VALCODER != 0) { // Отсекаем первое движение колеса
      if (VALCODER == 1) {
        if (VALCODER_OLD == 2) { OUT = 7; } // Следующее
        if (VALCODER_OLD == 3) { OUT = 8; } // Предидущее
      }
      if (VALCODER == 2) {
        if (VALCODER_OLD == 3) { OUT = 7; } // Следующее
        if (VALCODER_OLD == 1) { OUT = 8; } // Предидущее
      }
      if (VALCODER == 3) {
        if (VALCODER_OLD == 1) { OUT = 7; } // Следующее
        if (VALCODER_OLD == 2) { OUT = 8; } // Предидущее
      }
    }
    VALCODER_OLD = VALCODER;
  }

  // Вывод данных
  if (OUT < 7){ // Кнопки
     if      (OUT == 0) { DDRB = 0;          }
     else if (OUT == 1) { DDRB = 0b00000001; }
     else if (OUT == 2) { DDRB = 0b00000010; }
     else if (OUT == 3) { DDRB = 0b00000100; }
     else if (OUT == 4) { DDRB = 0b00001000; }
     else if (OUT == 5) { DDRB = 0b00010000; }
     else if (OUT == 6) { DDRB = 0b00100000; }
     PORTB = 0;
  }
  else { // Валкодер
    if (OUT == 7) { DDRB = 0b01000000; } else { DDRB = 0b10000000; }
    PORTB = 0;
    delay (120);
    DDRB = 0;
    delay (40);
  }
}

}