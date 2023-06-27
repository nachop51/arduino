#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define LED_ROWS 8
#define LED_COLS 8

#define PIN 12 // PIN para controlar la Matriz LED

// Definicion de pines de botones
#define B1 2
#define B2 3
#define B3 4
#define B4 5

#define ROCK 0
#define PAPER 1
#define SCISSORS 2

#define USER 0
#define PIXO 1
#define DRAW 2

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(LED_ROWS, LED_COLS, PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

const uint8_t options[3][8][8] = {
    {{0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 1, 1, 0, 0, 0},
     {0, 0, 1, 1, 1, 1, 0, 0},
     {0, 0, 1, 1, 1, 1, 0, 0},
     {0, 0, 1, 1, 1, 1, 0, 0},
     {0, 0, 1, 1, 1, 1, 0, 0},
     {0, 0, 0, 1, 1, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0},
     {0, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 0},
     {0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0}},
    {{1, 1, 1, 0, 0, 1, 1, 1},
     {1, 0, 1, 0, 0, 1, 0, 1},
     {1, 1, 1, 0, 0, 1, 1, 1},
     {0, 0, 0, 1, 1, 0, 0, 0},
     {0, 0, 1, 1, 1, 1, 0, 0},
     {0, 1, 1, 0, 0, 1, 1, 0},
     {1, 1, 0, 0, 0, 0, 1, 1},
     {1, 0, 0, 0, 0, 0, 0, 1}}};

uint8_t *random_colors()
{
  return matrix.Color(random(200, 255), random(0, 40), random(80, 210));
}

void print_string(char *string)
{

  int len = 0;

  while (string[len])
    len++;

  len = -len * 6;

  for (int i = 8; i > len; i--)
  {
    matrix.setTextColor(random_colors());
    matrix.setCursor(i, 1);
    matrix.print(string);
    matrix.show();
    matrix.clear();
    delay(100);
  }
}

void result(int winner)
{
  if (winner == PIXO)
    print_string("P wins");
  else if (winner == USER)
    print_string("U win :)");
  else
    print_string("DRAW");
  delay(500);
}

void setup()
{
  matrix.begin();
  matrix.setBrightness(20);

  randomSeed(analogRead(A0));

  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);
}

void loop()
{

  matrix.setTextColor(random_colors());
  print_string("RPS");

  uint8_t *current_color = random_colors();

  int user = -1;
  int pixo = random(0, 3);
  int winner = -1;

  while (user == -1)
  {
    if (digitalRead(B1) == HIGH)
    {
      user = ROCK;
    }
    else if (digitalRead(B2) == HIGH)
    {
      user = PAPER;
    }
    else if (digitalRead(B3) == HIGH)
    {
      user = SCISSORS;
    }
    else if (digitalRead(B4) == HIGH)
    {
    }
    delay(150);
  }

  print_string("Pixo:");

  for (int row = 0; row < LED_ROWS; row++)
  {
    for (int col = 0; col < LED_COLS; col++)
    {
      if (options[pixo][row][col] == 1)
      {
        matrix.drawPixel(row, col, current_color); // Set color to red
      }
      else
      {
        matrix.drawPixel(row, col, matrix.Color(0, 0, 0)); // Set color to black (unlit)
      }
    }
  }

  matrix.show();
  delay(1500);

  if (user == pixo)
    winner = DRAW;
  else
  {
    if (user == ROCK)
    {
      if (pixo == PAPER)
        winner = PIXO;
      else
        winner = USER;
    }
    else if (user == PAPER)
    {
      if (pixo == ROCK)
        winner = USER;
      else
        winner = PIXO;
    }
    else if (user == SCISSORS)
    {
      if (pixo == ROCK)
        winner = PIXO;
      else
        winner = USER;
      print_string("aaaa");
    }
  }

  result(winner);
}
