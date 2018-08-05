/*!
 *  \file      display.cpp
 *
 *  \brief     Функции для работы с дисплеем
 *
 *  \detailed  
 *
 *  \author    Андрей Алексеев <alekseev@thinking-house.ru
 *
 *  \copyright Лаборатория Умного Дома (ИП Алексеев А.П.), 2018. Все права защищены.
 */

//! Подключение заголовочных файлов
#include "display.h"

//! Макроопределения

//! Локальные данные 
UTFT myGLCD(TFT01_70,38,39,40,41);
UTFT_SdRaw myFiles(&myGLCD); 
SdFat      mySD; 
//! Локальные функции
void clear_screen()
{
  myGLCD.clrScr();
  delay(200);
  myGLCD.setColor(VGA_BLACK);
  myGLCD.setBackColor(VGA_BLACK);
  //myGLCD.fillScr(VGA_WHITE);
  myGLCD.setFont(Grotesk);
}
//! Глобальные функции
void init_display()
{
  Serial.println(F("TFT LCD Init"));

  myGLCD.InitLCD();

  clear_screen();
  mySD.begin(23);
}
//
void show_bmp()
{
    myFiles.load(0,0,800,480,"1.raw"); // выводим на дисплей картинку начиная с координаты 0,0 размером 320,240 из файла image_320x240_a.raw
}

void print_card_info(unsigned long card_id)
{
  String mess = "Card ID =" + String(card_id) + " not found";
  clear_screen();
  myGLCD.setColor(SERVICE_MESSAGE_COLOR);
  myGLCD.print(mess, CENTER, 32*3);
  myGLCD.print("", CENTER, 32*4);
  myGLCD.print("Press any key...", CENTER, 32*5);
}

void print_message(String mess) 
{
  clear_screen();
  myGLCD.setColor(SERVICE_MESSAGE_COLOR);
  myGLCD.print(mess, CENTER, 256);
}

void print_points(int block_time)
{
  clear_screen();
  myGLCD.setColor(SERVICE_MESSAGE_COLOR);
  myGLCD.print("Your card is blocked for "  + String(block_time)+ " sec", CENTER, 256);
  myGLCD.print("", CENTER, 288);
  myGLCD.print("Your current position is " + String(get_position_points()), CENTER, 320);
}

int print_question(question_t question)
{
  String answers[4];
  int true_answer = 0;
  int8_t answer_numbers[4];
  int8_t temp_number;
  char str_word[32];
  int8_t i;
  int16_t string_counter = 0;
  int j;

  //1
  temp_number = random(0, 4);
  answer_numbers[0] = temp_number;
  if (question.answers[answer_numbers[0]].is_true)
  {
    true_answer = 1;
    Serial.println("Верный ответ #" + String(1));
  }
  //2
  while(1)
  {
    temp_number = random(0, 4);
    if(temp_number != answer_numbers[0])
    {
      answer_numbers[1] = temp_number;
      if (question.answers[answer_numbers[1]].is_true)
      {
        true_answer = 2;
        Serial.println("Верный ответ #" + String(2));
      }
      break;
    }
  }
  //3
  while(1)
  {
    temp_number = random(0, 4);
    if(temp_number != answer_numbers[0] && temp_number != answer_numbers[1])
    {
      answer_numbers[2] = temp_number;
      if (question.answers[answer_numbers[2]].is_true)
      {
        true_answer = 3;
        Serial.println("Верный ответ #" + String(3));
      }
      break;
    }
  }
  //4
  while(1)
  {
    temp_number = random(0, 4);
    if(temp_number != answer_numbers[0] && temp_number != answer_numbers[1] && temp_number != answer_numbers[2])
    {
      answer_numbers[3] = temp_number;
      if (question.answers[answer_numbers[3]].is_true)
      {
        true_answer = 4;
        Serial.println("Верный ответ #" + String(4));
      }
      break;
    }
  }
  Serial.println("Порядок вывода ответов:" + String(answer_numbers[0]) + "," + String(answer_numbers[1]) + "," + String(answer_numbers[2]) + "," + String(answer_numbers[3]));
  clear_screen();

  Serial.print("Q:");Serial.println(question.text);
  Serial.print("A1:");Serial.println(question.answers[answer_numbers[0]].text);
  Serial.print("A2:");Serial.println(question.answers[answer_numbers[1]].text);
  Serial.print("A3:");Serial.println(question.answers[answer_numbers[2]].text);
  Serial.print("A4:");Serial.println(question.answers[answer_numbers[3]].text);
  Serial.print("T:");Serial.println(String(true_answer));

  myGLCD.setColor(QUESTION_TEXT_COLOR);
  j = 0;
  string_counter += QUESTION_TEXT_Y;
  int16_t count = String(question.text).length();
  if (String(question.text).startsWith("PICTURE="))
  {
    Serial.println("Вопрос это картинка");
    for(i = 8; i < count; i++, j++)
    {
      str_word[j] = question.text[i];
    }
    str_word[j] = '\0';
    String filename = String(get_card_now_class()) + "/" + String(str_word) + ".raw";
    Serial.println(filename);
    myFiles.load(190,5,420,240,filename.c_str());
  }
  else
  {
    Serial.println("Вопрос это текст");
    while(j < count)
    {
      for(i = 0; i < 31; i++)
      {
        str_word[i] = question.text[j];
        j++;
      }
      str_word[31] = '\0';
      Serial.print("Строка вопроса:");Serial.println(str_word);
      myGLCD.print(String(str_word), QUESTION_TEXT_X, string_counter);
      string_counter += 48 + 5;
      if (string_counter > 260)
        break;
    }
  }

  myGLCD.setColor(ANSWER_TEXT_COLOR);

  //----Ответ 1
//  for(i = 0; i < 31; i++)
//  {
//    str_word[i] = question.answers[answer_numbers[0]].text[i];
//  }
//  str_word[i] = '\0';
  myGLCD.print("A-" + String(question.answers[answer_numbers[0]].text), ANSWER1_TEXT_X, ANSWER1_TEXT_Y);

  //----Ответ 2
//  for(i = 0; i < 31; i++)
//  {
//    str_word[i] = question.answers[answer_numbers[1]].text[i];
//  }
//  str_word[i] = '\0';
  myGLCD.print("B-" + String(question.answers[answer_numbers[1]].text), ANSWER2_TEXT_X, ANSWER2_TEXT_Y);

  //----Ответ 3
//  for(i = 0; i < 31; i++)
//  {
//    str_word[i] = question.answers[answer_numbers[2]].text[i];
//  }
//  str_word[i] = '\0';
  myGLCD.print("C-" + String(question.answers[answer_numbers[2]].text), ANSWER3_TEXT_X, ANSWER3_TEXT_Y);

  //----Ответ 4
//  for(i = 0; i < 31; i++)
//  {
//    str_word[i] = question.answers[answer_numbers[3]].text[i];
//  }
//  str_word[i] = '\0';
  myGLCD.print("D-" + String(question.answers[answer_numbers[3]].text), ANSWER4_TEXT_X, ANSWER4_TEXT_Y);

  return true_answer;
}

void show_win()
{
  myFiles.load(160,120,420,240, "0.raw");
}

