/*
 *
 */

// include da Biblioteca de Controle de LCD
#include <LiquidCrystal.h>
#include <Time.h>

const int disarmPin = 13;

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

unsigned int lastRefresh = 0;
unsigned int bombTime = 1800;
unsigned int gameEnd = now() + bombTime;

void setup() {
  // Cria cada bloco de matriz como um char personalizado para formar os números
  for (int i = 0; i < 8; i++) {
    lcd.createChar(i, customChars[i]);
  }
  lcd.begin(20, 4);
  Serial.begin(9600);
  pinMode(disarmPin, INPUT);
  attachInterrupt(0, disarm, RISING);
}

void customNumber(int num, int col, int row) {
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

void customDot(int col, int row) {
  lcd.setCursor(col, row);
  lcd.write(4);
  lcd.setCursor(col, row + 1);
  lcd.write(4);
}

void refreshTimer() {
  if (lastRefresh != now()) {
    lastRefresh = now();
    unsigned int timer = gameEnd - lastRefresh;
    customNumber(minute(timer) / 10, 2, 0);
    customNumber(minute(timer) % 10, 5, 0);
    customDot(9, 0);
    customNumber(second(timer) / 10, 11, 0);
    customNumber(second(timer) % 10, 14, 0);
  }
}

void disarm() {
  unsigned int disarmStarted = now();
  while (digitalRead(disarmPin)) {
    refreshTimer();
  }
}

void loop()
{
  refreshTimer();
  Serial.println(gameEnd);
  Serial.println(lastRefresh);
  Serial.println(now());
}
