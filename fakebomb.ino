/*
 *
 */

// include da Biblioteca de Controle de LCD
#include <LiquidCrystal.h>
#include <Time.h>



/*  Inicializa a Interface de LCD
 *  Listagem dos Pinos Utilizados:
 *  Pino 07 -  RS - LCD04
 *  Pino 08 -  EN - LCD06
 *  Pino 09 - DB4 - LCD11
 *  Pino 10 - DB5 - LCD12
 *  Pino 11 - DB6 - LCD13
 *  Pino 12 - DB7 - LCD14
 */
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// the 8 arrays that form each segment of the custom numbers
byte customChars[8][8] = {
  {
    B00111,
    B01111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  }, {
    B11111,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  }, {
    B11100,
    B11110,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  }, {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B01111,
    B00111
  }, {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111
  }, {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11110,
    B11100
  }, {
    B11111,
    B11111,
    B11111,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111
  }, {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  }
};

const int disarmPin = 2;
unsigned int disarmTime = 17;
unsigned int bombTime = 1800;
unsigned int bombDisarmed = 0;
unsigned int bombExploded = now() + bombTime;


void setup() {

  // Setup Interrupts
  cli();//stop interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();

  // Cria cada bloco de matriz como um char personalizado para formar os números
  for (int i = 0; i < 8; i++) {
    lcd.createChar(i, customChars[i]);
  }
  lcd.begin(20, 4);
  Serial.begin(9600);
  pinMode(disarmPin, OUTPUT);
  attachInterrupt(0, defuse, CHANGE);
}

void drawNumber(int num, int col, int row) {
  lcd.setCursor(col, row);
  switch (num) {
    case 0:
      lcd.write(byte(0));
      lcd.write(1);
      lcd.write(2);
      lcd.setCursor(col, row + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
      break;
    case 1:
      lcd.write(1);
      lcd.write(2);
      lcd.print(' ');
      lcd.setCursor(col, row + 1);
      lcd.write(4);
      lcd.write(7);
      lcd.write(4);
      break;
    case 2:
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(col, row + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(4);
      break;
    case 3:
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(col, row + 1);
      lcd.write(4);
      lcd.write(4);
      lcd.write(5);
      break;
    case 4:
      lcd.write(3);
      lcd.write(4);
      lcd.write(7);
      lcd.setCursor(col, row + 1);
      lcd.print("  ");
      lcd.write(7);
      break;
    case 5:
      lcd.write(3);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(col, row + 1);
      lcd.write(4);
      lcd.write(4);
      lcd.write(5);
      break;
    case 6:
      lcd.write(byte(0));
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(col, row + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
      break;
    case 7:
      lcd.write(1);
      lcd.write(1);
      lcd.write(2);
      lcd.setCursor(col, row + 1);
      lcd.print("  ");
      lcd.write(7);
      break;
    case 8:
      lcd.write(byte(0));
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(col, row + 1);
      lcd.write(3);
      lcd.write(4);
      lcd.write(5);
      break;
    case 9:
      lcd.write(byte(0));
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(col, row + 1);
      lcd.print("  ");
      lcd.write(7);
      break;
    default:
      break;
  }
}

void drawDot(int col, int row) {
  lcd.setCursor(col, row);
  lcd.write(4);
  lcd.setCursor(col, row + 1);
  lcd.write(4);
}

void drawClock(unsigned int clockTime) {
  drawNumber(minute(clockTime) / 10, 2, 0);
  drawNumber(minute(clockTime) % 10, 5, 0);
  drawDot(9, 0);
  drawNumber(second(clockTime) / 10, 11, 0);
  drawNumber(second(clockTime) % 10, 14, 0);
  if (digitalRead(disarmPin)) {
    unsigned int disarmprogress = bombDisarmed - now();    
    lcd.setCursor(disarmprogress,3);
    lcd.write(7);
  }
}

void defuse() {
  if (digitalRead(disarmPin)) {
    bombDisarmed = now() + disarmTime;
    lcd.setCursor(3, 2);
    lcd.print("Desarmando: ");
    lcd.setCursor(0, 3);
    lcd.print('[');
    lcd.setCursor(19, 3);
    lcd.print(']');
  } else {
    bombDisarmed = 0;
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");
  }
}

ISR(TIMER1_COMPA_vect) {
  unsigned int timer = bombExploded - now();
  if (timer == 0) {
  } else {
    drawClock(timer);
  }
}

void loop() {
  //digitalWrite(disarmPin, HIGH);
}
