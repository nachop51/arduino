// Librerias
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR // Hace feliz a Arduino :)
#define PSTR
#endif

#define COLOR_CABEZA matrix.Color(255, 0, 255)
#define COLOR_CUERPO matrix.Color(255, 0, 128)
#define COLOR_MANZANA matrix.Color(0, 255, 0)
#define TIEMPO_JUEGO 350

#define PIN 12 // PIN para controlar la Matriz LED
// Definicion de pines de botones
#define B1 2
#define B2 3
#define B3 4
#define B4 5
// continuar con los demas botones

// Definicion de la Matriz LED
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

// Creemos dos variables. Una llamada X y otra Y. Vamos a inicializarlas en 0.
int X = 4;
// Crea la variable Y tu!
int Y = 4;

// Definamos la direccion y origen de nuestra serpiente
int dir = 0;
// DIR
/*     1
    4     2
       3
*/
int snakeHead[2] = {4, 4};

// Definamos el array que conforma el cuerpo de la serpiente
int snakeBody[64][2] = {};
int bodyCount = 1;

// Definamos la ubicacion de nuestra manzana
int apple[2] = {0, 0};

// Definamos la variable de status del juego
boolean inGame = true;

// SETUP
void setup()
{

  // Para comenzar a programar la Matriz LED debemos iniciarla. Escribe la instruccion para iniciar la Matriz!
  matrix.begin();
  // Ahora que nuestra Matriz LED esta iniciada,debemos configurar el brillo de los leds. Escribe la instruccion para configurar el brillo!
  matrix.setBrightness(20);

  // Configuracion de los botones como entrada
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);

  randomSeed(analogRead(A0)); // random seed for random function

  // Iniciemos los valores aleatorios de nuestra manzana
  apple[0] = random(0, 8);
  apple[1] = random(0, 8);
}

void delay_movement(int t)
{
  long t_i = millis();
  long t_f = t_i + t;

  while (millis() < t_f)
  {
    if (digitalRead(B1) == HIGH && dir != 3)
    {
      dir = 1;
    }
    else if (digitalRead(B2) == HIGH && dir != 4)
    {
      dir = 2;
    }
    else if (digitalRead(B3) == HIGH && dir != 1)
    {
      dir = 3;
    }
    else if (digitalRead(B4) == HIGH && dir != 2)
    {
      dir = 4;
    }
  }
}

// Procedimiento para dibujar el cuerpo de la serpiente
void drawSnake()
{
  for (int i = 1; i <= bodyCount; i++)
  {
    matrix.drawPixel(snakeBody[i][0], snakeBody[i][1], COLOR_CUERPO); // Dibujamos el pixel de coordenadas X e Y el color blanco
  }
  matrix.drawPixel(snakeHead[0], snakeHead[1], COLOR_CABEZA); // Dibujamos el pixel de coordenadas X e Y el color blanco
  matrix.show();                                              // Imprimimos en la matriz
}

// Procedimiento para actualizar la posicion de la serpiente
void bodyUpdate()
{
  for (int i = bodyCount; i > 0; i--)
  {
    snakeBody[i][0] = snakeBody[i - 1][0];
    snakeBody[i][1] = snakeBody[i - 1][1];
  }
  snakeBody[0][0] = snakeHead[0];
  snakeBody[0][1] = snakeHead[1];
}

// Funcion para chequear si un objeto se encuentra en el cuerpo de nuestra serpeiente
boolean onSnakeBody(int var[2])
{
  boolean notOverlaped = true;
  while (notOverlaped)
  {
    for (int i = 0; i < bodyCount; i++)
    {
      if (snakeBody[i][0] == var[0] and snakeBody[i][1] == var[1])
      {
        return true;
      }
      else if (i == bodyCount - 1)
      {
        notOverlaped = false;
      }
    }
  }
  return false;
}

// Funcion para chequear si nuestra cabeza choco contra el cuerpo
boolean snakeCollide()
{
  boolean notOverlaped = true;
  if (bodyCount == 1)
  {
    return false;
  }
  while (notOverlaped)
  {
    for (int i = 1; i <= bodyCount; i++)
    {
      if (snakeBody[i][0] == snakeHead[0] and snakeBody[i][1] == snakeHead[1])
      {
        return true;
      }
      else if (i == bodyCount)
      {
        notOverlaped = false;
      }
    }
  }
  return false;
}

// Procedimiento para cambiar la ubicacion de la manzana
void changeApple()
{
  while (onSnakeBody(apple))
  {
    apple[0] = random(0, 8);
    apple[1] = random(0, 8);
  }
}

// Procedimiento para dibujar la manzana
void drawApple()
{
  if (snakeHead[0] == apple[0] and snakeHead[1] == apple[1])
  {
    changeApple();
    bodyCount++;
  }
  matrix.drawPixel(apple[0], apple[1], COLOR_MANZANA); // Dibujamos el pixel de coordenadas X e Y el color blanco
}

// Procedimiento para reiniciar las variables del juego
void resetGame()
{
  Y = 4;
  X = 4;
  dir = 0;
  snakeHead[0] = 4;
  snakeHead[1] = 4;
  bodyCount = 1;
  for (int i = 0; i < bodyCount; i++)
  {
    snakeBody[i][0] = 0;
    snakeBody[i][1] = 0;
  }
  apple[0] = random(0, 8);
  apple[1] = random(0, 8);
  inGame = true;
}

void loop()
{

  // En el loop tendremos un loop con la condicion de status de juego.
  // Mientras esta variable sea verdadera, estaremos jugando.

  while (inGame)
  {
    // Chequeamos si algun boton se presiono. Si lo presionan, entonces cambiamos la direccion de la cabeza.
    if (digitalRead(B1) == HIGH && dir != 3)
    {
      dir = 1;
    }
    else if (digitalRead(B2) == HIGH && dir != 4)
    {
      dir = 2;
    }
    else if (digitalRead(B3) == HIGH && dir != 1)
    {
      dir = 3;
    }
    else if (digitalRead(B4) == HIGH && dir != 2)
    {
      dir = 4;
    }

    // Condicionantes para modificar el movimiento de la serpiente.
    if (dir == 1)
    {
      if (Y > 0)
      {
        Y--;
        snakeHead[1] = Y;
      }
      else
      {
        inGame = false;
      }
    }
    else if (dir == 2)
    {
      if (X < 7)
      {
        X++;
        snakeHead[0] = X;
      }
      else
      {
        inGame = false;
      }
    }
    else if (dir == 3)
    {
      if (Y < 7)
      {
        Y++;
        snakeHead[1] = Y;
      }
      else
      {
        inGame = false;
      }
    }
    else if (dir == 4)
    {
      if (X > 0)
      {
        X--;
        snakeHead[0] = X;
      }
      else
      {
        inGame = false;
      }
    }

    // Actualizamos la posicion de la serpiente
    bodyUpdate();
    // Dibujamos la serpiente
    drawSnake();
    // Chequeamos si la serpiente no colisiono con ella misma
    if (snakeCollide())
    {
      inGame = false;
    }
    // Dibujamos la manzana
    drawApple();

    // Mostramos todos los dibujos
    matrix.show();
    delay_movement(TIEMPO_JUEGO); // Delay de tiempo para hacer lento el movimiento
    matrix.clear();               // Limpiamos la matriz
  }
  // Fin del juego. Si el codigo llega hasta aqui, es porque el jugador perdio el juego

  matrix.fillScreen(matrix.Color(255, 0, 0)); // Encendemos la pantalla en rojo
  matrix.show();
  delay_movement(1000);
  matrix.clear();
  matrix.setTextColor(COLOR_MANZANA); // Selecciona el color. En este caso esta seleccionado el color rojo, entre los paréntesis se ubican los valores R,G,B (Red, Blue, Green) el máximo que se puede colocar de cada uno es 255, podemos jugar con esos valores para crear colores nuevos.

  // Escribimos el puntaje obtenido.
  for (int i = 4; i > -10; i--)
  {
    matrix.setCursor(i, 1); // Se para en la posición del pixel i, 1

    matrix.print(bodyCount - 1); // Valor final obtenido

    // Una vez que programada a la Matriz LED, le debemos dar la orden para mostrar que le programamos.
    matrix.show();  // Mostrar los cambios en la Matriz LED
    matrix.clear(); // Limpia la matriz, apaga todos los pixeles.

    delay_movement(200);
  }
  // Reseteamos el juego
  resetGame();
}
