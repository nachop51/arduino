#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR
#endif
#define PIN 12 // pin of the matrix led
typedef uint16_t Color;

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

// Global variables
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

// This array will contain the directions
// which the player must press
int sequence[50] = {0};
int points = 0;

void printNumber(int number) {
  // Given a number, print it out to the screen
  for (int i = 4; i > -10; i--) {
    matrix.setCursor(i, 1);
    matrix.print(number);
    matrix.show();
    matrix.clear();
    delay(250);
  }
}

void setup() {
  // This code only runs once, at the start of the program.
  Serial.begin(115200);

  matrix.begin();
  matrix.setBrightness(20);
  matrix.clear();
  matrix.show();

  // Map the pins to the buttons
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);

  // Set random seed for better random number generation
  randomSeed(analogRead(A0));
}

Color chooseColor(int direction) {
  // Set the colors of each one of the buttons
  // You have to check what is the direction
  // And return a color based on that.
  // Example: if the direction is UP then return RED
  if (direction == UP)
    return RED;
  else if (direction == RIGHT)
    return YELLOW;
  else if (direction == DOWN)
    return BLUE;
  else
    return GREEN;
}

void printButton(int direction) {
  // Based on a direction, print a button
  // You have to use the chooseColor function that you declared
  // right above, to set the color of the current button.
  // Remember to use the global variable 'buttons' that is an array of size [4][8][8]
  // You have to use the matrix variable, and one if its methods.

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (buttons[direction][x][y] == 1)
        matrix.drawPixel(x, y, chooseColor(direction));
      else
        matrix.drawPixel(x, y, 0);
    }
  }

  // Once you finish setting the configuration of
  // the pixels for the screen, show them, and wait, clear it again.
  matrix.show();
  delay(600);
  matrix.clear();
  matrix.show();
  delay(150);
}

void nextRound() {
  // The purpose of this function is to calculate
  // the next button that the player will have to memorize
  // Then, print the sequence, so the player can reproduce it :)
  // Hint: use the random function and the printButton
  // Also, you must save the calculated number in the sequence array.
  // The variable points and the sequence array, do they have a relationship?

  sequence[points] = random(0, 4);

  for (int i = 0; i <= points; i++) 
    printButton(sequence[i]);

  // sequence[] -> {RIGHT, DOWN, LEFT, UP, UP, UP, LEFT, DOWN}
}

int waitForButton() {
  // The idea of this function is to
  // wait for the button to get pressed
  // Then return the button pressed.
  // To achieve this, you will need to create
  // an infinite loop.

  // Look at this example:
  /*
      0    ->          B1 - UP
    3   1  ->  B4 - LEFT    B2 - RIGHT
      2    ->         B3 - DOWN
        Those are the macros
    defined at the start of the file :)
  */
  while (true) { // While button not pressed
    if (digitalRead(B1) == HIGH) {
      return UP;
    } else if (digitalRead(B2) == HIGH) {
      return RIGHT;
    } else if (digitalRead(B3) == HIGH) {
      return DOWN;
    } else if (digitalRead(B4) == HIGH) {
      return LEFT;
    }
  }
}

void loop() {
  // Well, is time to use all the function created above :)

  // First, you will need to generate and print the sequence.
  nextRound();

  // Loop over the array, with the condition of:
  // currentIndex less or equal than the points
  // Inside the loop, wait for the next button to get pressed
  // once the button is pressed, don't forget to show it
  // Then... is the direction of the button pressed equal to
  // the current direction in the sequence?
  // If not, then game over :(, show a red screen, print the score
  // and reset the game (set points to 0 and return from the function)

  for (int currentIndex = 0; currentIndex <= points; currentIndex++) {
    int direction = waitForButton();
    printButton(direction);
    
    if (direction != sequence[currentIndex]) {
      matrix.fillScreen(RED);
      matrix.show();
      delay(200);
      printNumber(points);
      points = 0;
      return;
    }
  }

  // Once the loop ended the player has reached the
  // end of the current sequence. Show a green screen 
  // and increment his points by 1 to congrats the player :D

  points += 1;

  matrix.fillScreen(GREEN);
  matrix.show();
  delay(500);
  matrix.clear();
  matrix.show();
  delay(200);
}

