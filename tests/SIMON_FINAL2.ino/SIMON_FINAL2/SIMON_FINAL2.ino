//Librerias
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR                        //Hace feliz a Arduino :)
#define PSTR
#endif
#define PIN 12                     //PIN para controlar la Matriz LED
//Definicion de pines de botones
#define B1 2
#define B2 3
#define B3 4
#define B4 5
//continuar con los demas botones


//Definicion de la Matriz LED
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN, NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

#define ROJO matrix.Color(255,0,0)
#define AMARILLO matrix.Color(255,255,0)
#define AZUL matrix.Color(0,0,255)
#define VERDE matrix.Color(0,255,0)

int seleccionarColor(int color) {
  if (color == 0) {
    return ROJO;
  }
  else if (color == 1) {
    return AMARILLO;
  }
  else if (color == 2) {
    return VERDE;
  }
  else if (color == 3) {
    return AZUL;
  }
}


//Definición del array de los botones del simon.
/*
   Dibujaremos los siguientes iconos

   01111110 111111
   10111101  1111  1            1
   11011011   11   11          11
   11100111        111        111
   11100111        111        111
   11011011        11    11    11
   10111101        1    1111    1
   01111110            111111
*/

int botonesSimon[4][8][8] = {
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0},
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
    {0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }

};

void dibujarBoton(int boton) {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (botonesSimon[boton][x][y] == 1) {

        matrix.drawPixel(x, y, seleccionarColor(boton));
      }
      else {
        matrix.drawPixel(x, y, matrix.Color(0, 0, 0));
      }
    }
  }
  matrix.show();
  delay(500);
  matrix.clear();
  matrix.show();
  delay(100);
}

//SETUP
void setup() {
  Serial.begin(115200);

  //Para comenzar a programar la Matriz LED debemos iniciarla. Escribe la instruccion para iniciar la Matriz!
  //<--- AQUI -- Inicia la Matriz LED
  matrix.begin();

  //Ahora que nuestra Matriz LED esta iniciada,debemos configurar el brillo de los leds. Escribe la instruccion para configurar el brillo!
  //<--- AQUI -- Configura el brillo de la Matriz LED
  matrix.setBrightness(100);
  //Ya configurmos el brillo de los leds.

  matrix.show();
  //Configuracion de los botones como entrada
  pinMode(B1, INPUT_PULLUP);

  //Seguir con los otros 3 botones
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);

  randomSeed(analogRead(A0));


}

int secuencia [50];
int puntos = 0;
void loop() {
  int boton = random(0, 4);
  secuencia[puntos] = boton;
  for (int i = 0; i <= puntos; i++) {
    dibujarBoton(secuencia[i]);
  }
  matrix.clear();
  matrix.show();
  boolean botonApretado = false;
  boolean botonCorrecto = false;

  for (int i = 0; i <= puntos ; i++) {
    botonApretado = false;
    botonCorrecto = false;
    while (!botonApretado) {
      if (digitalRead(B1) == HIGH) {

        botonApretado = true;
        if (secuencia[i] == 0) {
          botonCorrecto = true;
        }
      }
      if (digitalRead(B2) == HIGH) {
        botonApretado = true;
        if (secuencia[i] == 1) {
          botonCorrecto = true;
        }
      }
      if (digitalRead(B3) == HIGH) {
        botonApretado = true;
        if (secuencia[i] == 2) {
          botonCorrecto = true;
        }
      }
      if (digitalRead(B4) == HIGH) {
        botonApretado = true;
        if (secuencia[i] == 3) {
          botonCorrecto = true;
        }
      }
    }
    if (!botonCorrecto) {
      matrix.fillScreen(matrix.Color(255, 0, 0));
      matrix.show();
      delay(100);

      matrix.clear();
      matrix.show();
      break;
    }
    else {
      dibujarBoton(secuencia[i]);
      matrix.show();
     

      matrix.clear();
      matrix.show();
    }
  }
  if (!botonCorrecto) {
    matrix.fillScreen(matrix.Color(255, 0, 0));
    matrix.show();
    delay(200);

    matrix.clear();
    matrix.show();
    //Escribimos el puntaje obtenido.
    for (int i = 4; i > -10; i--) {
      matrix.setCursor(i, 1); //Se para en la posición del pixel i, 1


      matrix.print(puntos); //Valor final obtenido

      //Una vez que programada a la Matriz LED, le debemos dar la orden para mostrar que le programamos.
      matrix.show(); //Mostrar los cambios en la Matriz LED
      matrix.clear(); //Limpia la matriz, apaga todos los pixeles.

      delay(200);
    }
    puntos = 0;
  }
  else {
   

    matrix.clear();
    matrix.show();
    puntos++;
  }
  //Si llegaste hasta aqui significa que termino el tutorial del ejercicio 9, ahora completa el ejercicio y envia el codigo a tu kit PIXO.
  //Ahora te invitamos a cambiar el color del pixel y el tiempo de delay. Que sucede si lo aumentamos? Y si lo disminuimos?

}
