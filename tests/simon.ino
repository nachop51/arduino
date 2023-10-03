#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR
#endif
#define PIN 12 // pin of the matrix led

// Button mapping
#define B1 2
#define B2 3
#define B3 4
#define B4 5

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

#define RED matrix.Color(255, 0, 0)
#define YELLOW matrix.Color(255, 255, 0)
#define BLUE matrix.Color(0, 0, 255)
#define GREEN matrix.Color(0, 255, 0)

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  matrix.begin();

  matrix.setBrightness(20);

  matrix.show();

  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);

  randomSeed(analogRead(A0));
}

/*
   0 1 1 1 1 1 1 0 -> 111111
   1 0 1 1 1 1 0 1 ->  1111  1            1
   1 1 0 1 1 0 1 1 ->   11   11          11
   1 1 1 0 0 1 1 1 ->        111        111
   1 1 1 0 0 1 1 1 ->        111        111
   1 1 0 1 1 0 1 1 ->        11    11    11
   1 0 1 1 1 1 0 1 ->        1    1111    1
   0 1 1 1 1 1 1 0 ->            111111
*/
int buttons[4][8][8] = {
  {
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

int sequence[50] = {0};
int points = 0;

uint16_t chooseColor(uint16_t direction) {
  if (direction == UP)
    return RED;
  if (direction == RIGHT)
    return BLUE;
  if (direction == DOWN)
    return YELLOW;
  else
    return GREEN;
}

void printButton(int dir) {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      buttons[dir][x][y] == 1 ? matrix.drawPixel(x, y, chooseColor(dir)) : matrix.drawPixel(x, y, 0);
    }
  }

  matrix.show();
  delay(600);
  matrix.clear();
  matrix.show();
  delay(150);
}

void printSequence() {
  // Print
  for (int i = 0; i <= points; i++) {
    printButton(sequence[i]);
  }
  
  matrix.clear();
  matrix.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  int nextColor = random(0, 4);
  sequence[points] = nextColor;

  printSequence();
  delay(1000);
  for (int i = 4; i > -10; i--) {
    matrix.setCursor(i, 1); //Se para en la posición del pixel i, 1


    matrix.print(nextColor); //Valor final obtenido

    //Una vez que programada a la Matriz LED, le debemos dar la orden para mostrar que le programamos.
    matrix.show(); //Mostrar los cambios en la Matriz LED
    matrix.clear(); //Limpia la matriz, apaga todos los pixeles.

    delay(200);
  }
  delay(1000);
}

