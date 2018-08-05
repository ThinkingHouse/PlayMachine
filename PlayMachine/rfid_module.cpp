/*!
 *  \file      rfid_module.cpp
 *
 *  \brief     Функции для работы с картами
 *
 *  \detailed  
 *
 *  \author    Андрей Алексеев <alekseev@thinking-house.ru
 *
 *  \copyright Лаборатория Умного Дома (ИП Алексеев А.П.), 2018. Все права защищены.
 */

//! Подключение заголовочных файлов
#include "rfid_module.h"

//! Макроопределения

//! Локальные данные 
String input_string = "";         // a String to hold incoming data
boolean string_complete = false;  // whether the string is complete

//! Глобальные функции
void rfid_init()
{
  Serial1.begin(115200);
  pinMode(2, OUTPUT);
}

unsigned long wait_card_id()
{
  unsigned long result = 0;  // для храниения номера метки в десятичном формате
  digitalWrite(2, HIGH);
  Serial.println("wait_card_id ");
  while (Serial1.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial1.read();
    // add it to the inputString:
    input_string += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') 
    {
      string_complete = true;
    }
  }
  
  if (string_complete) 
  {
    result = strtoul(input_string.c_str(), NULL, 10);
    Serial.print("Card ID: ");
    Serial.println(result);
    // clear the string:
    input_string = "";
    string_complete = false;
  }
  digitalWrite(2, LOW);
  Serial.println(result);
  return result;
}

