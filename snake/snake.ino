#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifndef PSTR
#define PSTR
#endif

#define COLOR_CABEZA matrix.Color(255, 0, 255)
#define COLOR_CUERPO matrix.Color(255, 0, 128)
#define COLOR_MANZANA matrix.Color(0, 255, 0)

#define PIN 12 // Donde esta la matriz LED
// Botones
#define B1 2
#define B2 3
#define B3 4
#define B4 5

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

// Posicion inicial de la cabeza
int posX = 4;
int posY = 4;

// Direccion de la serpiente
int dir = 0;
/*
  1
4   2
  3
*/

int snakeHead[2] = {posX, posY};
int snakeBody[64][2] = {};
int bodyLength = 1;

int apple[2] = {0, 0};

bool playing = true;

void setup()
{
  matrix.begin();
}
