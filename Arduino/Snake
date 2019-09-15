#define NUM_LEDS 64
#include "FastLED.h"
#define PIN 10
CRGB leds[NUM_LEDS];

byte food_coord[2];
byte snake_coord[NUM_LEDS][2];
byte buffer_snake[2];
byte snake_size = 0;
String move_to = "up";
bool flag = false;

void food_gener()
{ 
  food_coord[0] = random(sqrt(NUM_LEDS));
  food_coord[1] = random(sqrt(NUM_LEDS));
}

void setup() {
  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(100);
  pinMode(13, OUTPUT);
  food_gener();
  for(byte i = 1; i < NUM_LEDS; i++)
  {
    snake_coord[i][0] = 10;
    snake_coord[i][1] = 10;
  }
  snake_coord[0][0] = 0;      //X
  snake_coord[0][1] = 0;      //Y
}

byte convert(byte x, byte y)
{
  return (y % 2 == 0) ? (y*8 + x) : (y*8 + 7 - x);
}

void print_food()
{
  leds[convert(food_coord[0], food_coord[1])] = CHSV(255, 255, 255);
}

void clear_field()
{
  for (byte i = 0; i < NUM_LEDS; i++ )
    leds[i] = CHSV(0, 255, 0);
}

void print_snake()
{
  for(byte i = 0; i < NUM_LEDS; i++)
    leds[convert(snake_coord[i][0], snake_coord[i][1])] = CHSV(96, 255, 100);
}

void eat_food()
{
  for(byte i = NUM_LEDS - 1; i > 0; i--)
    {
      snake_coord[i][0] = snake_coord[i - 1][0];
      snake_coord[i][1] = snake_coord[i - 1][1];
    }
  snake_coord[0][0] = food_coord[0];
  snake_coord[0][1] = food_coord[1];
  food_gener();
}

void find_food()
{
  if(snake_coord[0][1] == food_coord[1] && snake_coord[0][0] > food_coord[0])
    move_to = "left";
  if(snake_coord[0][1] == food_coord[1] && snake_coord[0][0] < food_coord[0])
    move_to = "right";
  if(snake_coord[0][0] == food_coord[0] && snake_coord[0][1] > food_coord[1])
    move_to = "down";
  if(snake_coord[0][0] == food_coord[0] && snake_coord[0][1] < food_coord[1])
    move_to = "up";
}

void check_next_step()
{
  if((snake_coord[0][1] == 7 && move_to == "up") && snake_coord[0][0] != 7)
    move_to = "right";
  else if((snake_coord[0][1] == 0 && move_to == "down") && snake_coord[0][0] != 0)
    move_to = "left";
  else if((snake_coord[0][0] == 7 && move_to == "right") && snake_coord[0][1] != 0)
    move_to = "down";
  else if((snake_coord[0][0] == 0 && move_to == "left") && snake_coord[0][1] != 7)
    move_to = "up";
  else
    find_food();
}

void print_smile(byte color = 145, byte bright = 120)
{
  byte color_eye = color;
  byte color_background = 0;
  
  clear_field();
  for (byte j = 0; j < NUM_LEDS; j++)
       leds[j] = CHSV(64, 255, 20);
  
  leds[convert(2, 0)] = CHSV(color, 255, bright);
  leds[convert(3, 0)] = CHSV(color, 255, bright);
  leds[convert(4, 0)] = CHSV(color, 255, bright);
  leds[convert(5, 0)] = CHSV(color, 255, bright);
  leds[convert(2, 7)] = CHSV(color, 255, bright);
  leds[convert(3, 7)] = CHSV(color, 255, bright);
  leds[convert(4, 7)] = CHSV(color, 255, bright);
  leds[convert(5, 7)] = CHSV(color, 255, bright);
  
  leds[convert(1, 6)] = CHSV(color, 255, bright);
  leds[convert(6, 6)] = CHSV(color, 255, bright);
  leds[convert(1, 1)] = CHSV(color, 255, bright);
  leds[convert(6, 1)] = CHSV(color, 255, bright);
  
  leds[convert(0, 2)] = CHSV(color, 255, bright);
  leds[convert(7, 2)] = CHSV(color, 255, bright);
  leds[convert(0, 3)] = CHSV(color, 255, bright);
  leds[convert(7, 3)] = CHSV(color, 255, bright);
  leds[convert(0, 4)] = CHSV(color, 255, bright);
  leds[convert(7, 4)] = CHSV(color, 255, bright);
  leds[convert(0, 5)] = CHSV(color, 255, bright);
  leds[convert(7, 5)] = CHSV(color, 255, bright);
  
  leds[convert(1, 3)] = CHSV(color, 255, bright);
  leds[convert(6, 3)] = CHSV(color, 255, bright);
  leds[convert(4, 2)] = CHSV(color, 255, bright);
  leds[convert(3, 2)] = CHSV(color, 255, bright);
  leds[convert(5, 2)] = CHSV(color, 255, bright);
  leds[convert(2, 2)] = CHSV(color, 255, bright);
  
  leds[convert(2, 5)] = CHSV(color_eye, 255, bright);
  leds[convert(5, 5)] = CHSV(color_eye, 255, bright);
  
  FastLED.show();
  delay(1000);
  leds[convert(5, 5)] = CHSV(color_eye, 255, 0);
  FastLED.show();
  delay(300);
  leds[convert(5, 5)] = CHSV(color_eye, 255, bright);
  FastLED.show();
  delay(5000);
}

void check_error()
{
  for(byte i = 1; i < NUM_LEDS; i++)
     if(snake_coord[0][0] == snake_coord[i][0] && snake_coord[0][1] == snake_coord[i][1])
     {
       print_smile();
       FastLED.show();
       delay(5000);
       asm("JMP 0");
     } 
}

void step_snake()
{
  for(byte i = NUM_LEDS - 1; i > 0; i--)
    if(snake_coord[i][0] != 10 && snake_coord[i][1] != 10)
    {
      snake_coord[i][0] = snake_coord[i - 1][0];
      snake_coord[i][1] = snake_coord[i - 1][1];
    }
  if(move_to == "up")
    snake_coord[0][1]++;
  else if(move_to == "down")
    snake_coord[0][1]--;
  else if(move_to == "left")
    snake_coord[0][0]--;
  else if(move_to == "right")
    snake_coord[0][0]++;
  check_error();
}

void snake()
{
  check_next_step();
  step_snake();
  if(snake_coord[0][0] == food_coord[0] && snake_coord[0][1] == food_coord[1])
    eat_food();
}



void loop()
{
  snake();
  clear_field();
  print_food();
  print_snake();
  FastLED.show();
  delay(100);         // speed run
}
