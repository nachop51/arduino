#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifndef PSTR
#define PSTR
#endif

// Matrix specs
#define MATRIX_BRIGHTNESS 20
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8

// Colors
#define COLOR_CABEZA matrix.Color(255, 0, 255)
#define COLOR_CUERPO matrix.Color(255, 0, 128)
#define COLOR_MANZANA matrix.Color(0, 255, 0)

#define PIN 12 // Donde esta la matriz LED

// Botones
#define B1 2
#define B2 3
#define B3 4
#define B4 5

// ¿Cada cuanto se mueve la serpiente? (ms)
#define DELAY 300

// Define directions
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

// Posicion inicial de la cabeza
#define INITIAL_X 4
#define INITIAL_Y 4

// Direccion de la serpiente
// The direction starts in 0 because the snake is not moving at the beginning
// So it won't move until the user presses a button
int dir = 0;
/*
  1
4   2
  3
*/

int snakeHead[2] = {INITIAL_X, INITIAL_Y};
int snakeBody[64][2] = {};
int bodyLength = 1;

int apple[2] = {0, 0};

bool isAlive = true;

void setup()
{
  matrix.begin();
  matrix.setBrightness(MATRIX_BRIGHTNESS);

  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);

  randomSeed(analogRead(A0));

  spawnApple();
}

bool onSnakeBody(int something[2])
{
  for (int i = 0; i < bodyLength; i++)
  {
    if (snakeBody[i][0] == something[0] && snakeBody[i][1] == something[1])
    {
      return true;
    }
  }
  return false;
}

void drawSomething(int something[2], uint32_t color)
{
  matrix.drawPixel(something[0], something[1], color);
}

void spawnApple()
{
  do
  {
    apple[0] = random(0, MATRIX_WIDTH);
    apple[1] = random(0, MATRIX_HEIGHT);
  } while (onSnakeBody(apple));
}

void delay_movement(int t)
{
  long t_i = millis();
  long t_f = t_i + t;

  while (millis() < t_f)
  {
    if (digitalRead(B1) == HIGH && dir != DOWN)
    {
      dir = UP;
    }
    else if (digitalRead(B2) == HIGH && dir != LEFT)
    {
      dir = RIGHT;
    }
    else if (digitalRead(B3) == HIGH && dir != UP)
    {
      dir = DOWN;
    }
    else if (digitalRead(B4) == HIGH && dir != RIGHT)
    {
      dir = LEFT;
    }
  }
}

void drawSnake()
{
  for (int i = 0; i < bodyLength; i++)
  {
    drawSomething(snakeBody[i], COLOR_CUERPO);
  }

  drawSomething(snakeHead, COLOR_CABEZA);

  matrix.show();
}

void updateSnakeBody()
{
  for (int i = bodyLength - 1; i > 0; i--)
  {
    snakeBody[i][0] = snakeBody[i - 1][0];
    snakeBody[i][1] = snakeBody[i - 1][1];
  }

  snakeBody[0][0] = snakeHead[0];
  snakeBody[0][1] = snakeHead[1];
}

void displayEndGame()
{
  matrix.fillScreen(matrix.Color(255, 0, 0));
  matrix.show();
  delay(1000);
  matrix.clear();

  for (int i = 4; i > -10; i--)
  {
    matrix.setCursor(i, 1);

    matrix.print(bodyLength - 1);

    matrix.show();  // Mostrar los cambios en la Matriz LED
    matrix.clear(); // Limpia la matriz, apaga todos los pixeles.

    delay(200);
  }
}

void resetGame()
{
  dir = 0;
  bodyLength = 1;
  snakeHead[0] = INITIAL_X;
  snakeHead[1] = INITIAL_Y;
  spawnApple();
  isAlive = true;
}

bool snakeCollide()
{
  for (int i = 1; i < bodyLength; i++)
    if (snakeHead[0] == snakeBody[i][0] && snakeHead[1] == snakeBody[i][1])
      return true;
  return false;
}

void loop()
{
  while (isAlive)
  {
    delay_movement(DELAY);
    matrix.clear();

    switch (dir)
    {
    case UP:
      if (snakeHead[1] == 0)
      {
        isAlive = false;
      }
      else
      {
        snakeHead[1]--;
      }
      break;
    case RIGHT:
      if (snakeHead[0] == MATRIX_WIDTH - 1)
      {
        isAlive = false;
      }
      else
      {
        snakeHead[0]++;
      }
      break;
    case DOWN:
      if (snakeHead[1] == MATRIX_HEIGHT - 1)
      {
        isAlive = false;
      }
      else
      {
        snakeHead[1]++;
      }
      break;
    case LEFT:
      if (snakeHead[0] == 0)
      {
        isAlive = false;
      }
      else
      {
        snakeHead[0]--;
      }
      break;
    }

    updateSnakeBody();
    drawSnake();

    if (snakeCollide())
    {
      isAlive = false;
    }

    // Check if the snake ate the apple
    if (snakeHead[0] == apple[0] && snakeHead[1] == apple[1])
    {
      bodyLength++;
      spawnApple();
    }

    drawSomething(apple, COLOR_MANZANA);

    matrix.show();
  }

  displayEndGame();
  resetGame();
}
