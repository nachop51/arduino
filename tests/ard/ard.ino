#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <pins_arduino.h>

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define LED_ROWS 8
#define LED_COLS 8

#define PIN 12  //PIN para controlar la Matriz LED

//Definicion de pines de botones
#define B1 2
#define B2 3
#define B3 4
#define B4 5

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(LED_ROWS, LED_COLS, PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

#define DELAY

const uint8_t heart_fill[LED_ROWS][LED_COLS] = {
  {0, 0, 1, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 0, 0, 0, 0}
};

const uint8_t heart[LED_ROWS][LED_COLS] = {
  {0, 0, 1, 1, 0, 0, 0, 0},
  {0, 1, 0, 0, 1, 0, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 1, 0, 0, 0},
  {0, 0, 1, 1, 0, 0, 0, 0}
};

void setup() {
  // put your setup code here, to run once:
  matrix.begin();
  matrix.clear(); //Limpia la matriz, apaga todos los pixeles.
  //Ahora que nuestra Matriz LED esta iniciada,debemos configurar el brillo de los leds. Escribe la instruccion para configurar el brillo!
  //<--- AQUI -- Configura el brillo de la Matriz LED
  matrix.setBrightness(20);
  //Generemos la semilla aleatoria
  randomSeed(analogRead(A0));
  matrix.setTextWrap(false);

  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);
}

uint8_t *random_colors() {
  return matrix.Color(random(200, 255), random(0, 40), random(80,210));
}

void print_string(char *string) {

  int len = 0;

  while (string[len])
    len++;

  len = -len * 6;

  for (int i = 8; i > len; i--) {
    matrix.setTextColor(random_colors());
    matrix.setCursor(i, 1);
    matrix.print(string);
    matrix.show();
    matrix.clear();
    delay(200);
  }
}

void loop() {
  // print_string("Hola marti <3");
  matrix.setTextColor(random_colors());
  print_string("Hola lu <3");
  // print_string("Holas");

  uint8_t *current_color = random_colors();
  uint8_t (*current_heart)[LED_ROWS][LED_COLS] = &heart_fill;

  while (1) {
    // current_color = random_colors();
    if (digitalRead(B1) == HIGH) {
      
    } else if (digitalRead(B2) == HIGH) {
      current_color = random_colors();
    } else if (digitalRead(B3) == HIGH) {
      if (current_heart == &heart_fill)
        current_heart = &heart;
      else
        current_heart = &heart_fill;
    } else if (digitalRead(B4) == HIGH) {
      break;
    }
    delay(200);

    for (int row = 0; row < LED_ROWS; row++) {
      for (int col = 0; col < LED_COLS; col++) {
        if ((*current_heart)[row][col] == 1) {
          matrix.drawPixel(row, col, current_color);  // Set color to red
        } else {
          matrix.drawPixel(row, col, matrix.Color(0, 0, 0));  // Set color to black (unlit)
        }
      }
    }
    matrix.show();
  }

  delay(1000); // 1 sec
}
