#include "rfid_module.h"

int8_t leds[8] = { 7, 6, 5, 4, 3, 2, A0, A1};
bool  snake[8] = { 1, 0, 0, 0, 0, 0, 0, 0};
void setup() 
{
  Serial.begin(115200);
  pinMode(8, INPUT);
  pinMode(A2, INPUT);
  SPI.begin();        // Init SPI bus
  rfid_init();
  for(int i = 0; i < 8; i++)
  {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void loop() 
{
  if (digitalRead(8) == HIGH)
  {
    unsigned long card_id = get_card_id();
    if (card_id == 0)
      return;
    Serial.println(card_id);
    delay(500);
  }
  
  if (digitalRead(A2) == LOW)
  {
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(leds[i], LOW);
    }
  }
  
  while (digitalRead(A2) == HIGH)
  {
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(leds[i], snake[i]);
    }
    for (int i = 0; i < 8; i++)
    {
      if (snake[i] == 1)
      {
        if (i == 7)
        {
          snake[i] = 0;
          snake[0] = 1;
        }
        else
        {
          snake[i] = 0;
          snake[i + 1] = 1;
        }
        break;
      }
    }
    delay(100);
  }

}
