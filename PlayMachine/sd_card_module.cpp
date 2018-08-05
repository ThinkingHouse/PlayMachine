/*!
 *  \file      questions.cpp
 *
 *  \brief     Функции для получения вопросов
 *
 *  \detailed  
 *
 *  \author    Андрей Алексеев <alekseev@thinking-house.ru
 *
 *  \copyright Лаборатория Умного Дома (ИП Алексеев А.П.), 2018. Все права защищены.
 */

//! Подключение заголовочных файлов
#include "sd_card_module.h"

//! Локальные макроопределения
#define MAX_CARD_COUNT 275

//! Локальные данные 
int16_t card_active_count=0;
card_t card_now;
active_card_t card_active_mass[MAX_CARD_COUNT];
SdFat SD;

//! Локальные функции

//! Глобальные функции
void sd_card_init()
{
  Serial.print("Initializing SD card...");
  if (!SD.begin(23)) 
  {
    Serial.println("initialization failed!");
    return -1;
  }
  Serial.println("initialization done.");

  for(int i=0; i < MAX_CARD_COUNT; i++)
  {
    card_active_mass[i].card_id = 0;
    card_active_mass[i].block_time = 0;
    card_active_mass[i].points = 0;
  }
}

question_t get_question(int8_t class_number, int num)
{
  File myFile;
  question_t result;
  answer_t answer_temp;
  char temp;
  String filename = String(class_number)+".txt";
  int counter = 0;
  int i = 0;
  // re-open the file for reading:
  myFile = SD.open(filename);
  if (myFile) 
  {
    Serial.println(filename);
    
      //
      if (num !=0)
      {
       
        while(1)
        {
          temp = myFile.read();
          if (temp == '$')
          {
            counter++;
            myFile.read();
            
          }
          if (counter == num)
          {
            myFile.read();
            break;
          }
        }
      }
      
      //Достаем вопрос
      temp = myFile.read();
      while (temp != '#')
      {
        result.text[i] = temp;
        i++;
        temp = myFile.read();
      }
      result.text[i] = '\0';

      //Достаем ответ 1
      i = 0;
      temp = myFile.read();
      while (temp != '#')
      {
        if (temp != '=')
        {
          result.answers[0].text[i] = temp;
        }
        else
        {
          result.answers[0].text[i] = '\0';
          i++;
          temp = myFile.read();
          if (temp == '1')
            result.answers[0].is_true = true;
          else
            result.answers[0].is_true = false;
        }
          
        i++;
        temp = myFile.read();
      }

      //Достаем ответ 2
      i = 0;
      temp = myFile.read();
      while (temp != '#')
      {
        if (temp != '=')
        {
          result.answers[1].text[i] = temp;
        }
        else
        {
          result.answers[1].text[i] = '\0';
          i++;
          temp = myFile.read();
          if (temp == '1')
            result.answers[1].is_true = true;
          else
            result.answers[1].is_true = false;
        }
          
        i++;
        temp = myFile.read();
      }

      //Достаем ответ 3
      i = 0;
      temp = myFile.read();
      while (temp != '#')
      {
        if (temp != '=')
        {
          result.answers[2].text[i] = temp;
        }
        else
        {
          result.answers[2].text[i] = '\0';
          i++;
          temp = myFile.read();
          if (temp == '1')
            result.answers[2].is_true = true;
          else
            result.answers[2].is_true = false;
        }
          
        i++;
        temp = myFile.read();
      }

      //Достаем ответ 4
      i = 0;
      temp = myFile.read();
      while (temp != '#')
      {
        if (temp != '=')
        {
          result.answers[3].text[i] = temp;
        }
        else
        {
          result.answers[3].text[i] = '\0';
          i++;
          temp = myFile.read();
          if (temp == '1')
            result.answers[3].is_true = true;
          else
            result.answers[3].is_true = false;
        }
          
        i++;
        temp = myFile.read();
      }
      
      i = 0;
    // close the file:
    myFile.close();
  } 
  else 
  {
    Serial.println("error opening " + filename);
  }

  return result;
}

int count_question(int class_number)
{
  File myFile;
  int i = 0;
  char temp;
  String filename = String(class_number)+".txt";
  int counter = 0;
  // re-open the file for reading:
  Serial.println("getQuestionCount файл - " + String(filename));
  myFile = SD.open(filename);
  if (myFile) 
  {
    // read from the file until there's nothing else in it:
    while (myFile.available()) 
    {
      if(myFile.read() == '$')
      {
        counter++;
      }
    }
    // close the file:
    myFile.close();
  } 
  else 
  {
    // if the file didn't open, print an error:
    Serial.println("error opening " + filename);
  }
  Serial.println("getQuestionCount Количество вопросов в файле - " + String(counter));
  return counter;
}

bool read_cards_from_file()
{
  File myFile;
  int i = 0;
  int j = 0;
  int card_num = 0;
  char file_str[60];
  char temp;
  String filename = "card.txt";
  myFile = SD.open(filename);
  if (myFile) 
  {
    while (myFile.available()) 
    {
      temp = myFile.read();
      if(temp != ';')
      {
        if(temp != '\n')
        {
          file_str[i] = temp;
        }
        i++;
      }
      else
      {
        file_str[i] = '\0';
        temp = myFile.read();
        char card_id[50];
        int8_t class_num;
        char fio[100];
        int t = 0;
        //Serial.println("file_str=" + String(file_str));
        //question_temp[questions_number] = String(questions_str);
        while(file_str[j] != '#')
        {
          card_id[t] = file_str[j];
          t++;
          j++;
        }
        card_id[t] = '\0';
        j++;
        class_num = (int)file_str[j]-48;
        j+=2;
        t = 0;
        while(file_str[j] != '\0')
        {
          fio[t] = file_str[j];
          t++;
          j++;
        }
        fio[t] = '\0';
        Serial.print("ID Now ");Serial.print(get_card_now_id());Serial.print("ID file ");Serial.print(card_id);
        if (strtoul(card_id,NULL,10) == get_card_now_id())
        {
          card_now.card = strtoul(card_id,NULL,10);
          card_now.name = String(fio);
          card_now.class_num = class_num;
          Serial.print("ID ");Serial.print(card_now.card);Serial.print(" FIO ");Serial.print(card_now.name);Serial.print(" class ");Serial.print(card_now.class_num);Serial.println("");
          return true;
        }
        i = 0;
        j = 0;
        t = 0;
        myFile.read();
      }
    }
    myFile.close();
  } 
  else 
  {
    // if the file didn't open, print an error:
    Serial.println("error opening " + filename);
  }
  return false;
}

int16_t check_card_block()
{
  int16_t result = 0;
  Serial.println("--- Проверка Блокировки Карты ------------------");
  for(int i=0; i < card_active_count + 1; i++)
  {
    Serial.print(i);Serial.print(".");Serial.print("card_id=");Serial.print(card_active_mass[i].card_id);Serial.print(" block_time=");Serial.println(card_active_mass[i].block_time);
    if (card_active_mass[i].card_id == card_now.card)
    {
      if (card_active_mass[i].block_time > 0)
      {
        result = card_active_mass[i].block_time;
        Serial.print("card_id блокирована=");Serial.print(card_active_mass[i].card_id);
        return result;
      }
      else
      {
        result = 0;
        Serial.print("Карта не блокирована=");Serial.print(card_active_mass[i].card_id);
        return result;
      }
    }
  }
  return result;
}

unsigned long get_card_now_id()
{
  return card_now.card;
}

int8_t get_card_now_class()
{
  return card_now.class_num;
}

String get_card_now_fio()
{
  return card_now.name;
}

void set_card_block()
{ 
  if (card_active_count != 0)
  {
    for(int i = 0; i < card_active_count + 1; i++) 
    {
      //Если карта уже юзалась
      if (card_active_mass[i].card_id == card_now.card)
      {
        card_active_mass[i].block_time = CARD_BLOCK_TIME;
        return;
      }
    }

    //Если карта не найдена
    card_active_mass[card_active_count + 1].card_id = card_now.card;
    card_active_mass[card_active_count + 1].block_time = CARD_BLOCK_TIME;
    card_active_mass[card_active_count + 1].points = 0;
    card_active_count++;
    return;
  }
  else
  {
    card_active_mass[0].card_id = card_now.card;
    card_active_mass[0].block_time = CARD_BLOCK_TIME;
    card_active_mass[0].points = 0;
    card_active_count++;
    return;
  }
}

void down_card_block(int time_n)
{ 
    int16_t time_temp;
    //Serial.println("down_card_block time_n=" + String(time_n));
    for(int i = 0; i < card_active_count + 1; i++) 
    {
      if(card_active_mass[i].block_time != 0)
      {
        time_temp = card_active_mass[i].block_time - time_n;
        if(time_temp > 0)
          card_active_mass[i].block_time = time_temp;
        else
          card_active_mass[i].block_time = 0;
      }
    }
}

void set_card_now_id(unsigned long card_id)
{
  card_now.card = card_id;
}

void reset_card_now()
{
  card_now.card = 0;
  card_now.name = "";
  card_now.class_num = -1;
}

void read_point_from_file()
{
  File myFile;
  int i = 0;
  int j = 0;
  int card_num = 0;
  char file_str[60];
  char temp;
  String filename = "points.txt";
  card_active_count = 0;
  myFile = SD.open(filename);
  if (myFile) 
  {
    while (myFile.available()) 
    {
      temp = myFile.read();
      if(temp != '$')
      {
        if(temp != '\n')
        {
          file_str[i] = temp;
        }
        i++;
      }
      else
      {
        file_str[i] = '\0';
        temp = myFile.read();
        char card_id[50];
        char point_str[8];
        int8_t class_num;
        int point;
        int t = 0;
        //Serial.println("file_str=" + String(file_str));
        //question_temp[questions_number] = String(questions_str);
        while(file_str[j] != '#')
        {
          card_id[t] = file_str[j];
          t++;
          j++;
        }
        card_id[t] = '\0';
        j++;
        t = 0;
        while(file_str[j] != '#')
        {
          point_str[t] = file_str[j];
          t++;
          j++;
        }
        point_str[t] = '\0';
        point = String(point_str).toInt();

        j++;
        class_num = (int)file_str[j]-48;
        Serial.print("ID ");Serial.print(String(card_id));Serial.print(" points ");Serial.print(point);Serial.print(" class ");Serial.print(class_num);Serial.println("");
        card_active_mass[card_active_count].card_id = strtoul(card_id,NULL,10);
        card_active_mass[card_active_count].points = point;
        card_active_mass[card_active_count].block_time = 0;
        card_active_count++;
        i = 0;
        j = 0;
      }
    }
    // close the file:
    myFile.close();
  } 
  else 
  {
    // if the file didn't open, print an error:
//    Serial.println("error opening " + filename);
//    for(int i = 0; i < card_count; i++) 
//    {
//        card_dict[i].points = 0;
//    }
  }
}

void save_points_to_file()
{
  SD.remove("points.txt");
  File myFile = SD.open("points.txt", FILE_WRITE);
  if (myFile) 
  {
    Serial.print("Writing to points.txt...");
    for(int i = 0; i < MAX_CARD_COUNT; i++)
    {
      if(card_active_mass[i].card_id != 0 && card_active_mass[i].card_id > 10000)
      {
        String text = String(card_active_mass[i].card_id) + "#" + String(card_active_mass[i].points) + "#" + String(card_now.class_num) + "#$";
        Serial.print("Пишем в файл "); Serial.println(text);
        myFile.print(text);
        if (i != card_active_count)
        {
          myFile.println("");
        }
      }
    }
    myFile.close();
    Serial.println("done.");
  } 
  else 
  {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void add_point()
{
    for(int i = 0; i < MAX_CARD_COUNT; i++)
    {
      if (card_active_mass[i].card_id == card_now.card)
      {
        card_active_mass[i].points++;
        Serial.print("Рейтинг повышен ID="); Serial.print(card_active_mass[i].card_id);Serial.print(" Очки="); Serial.println(card_active_mass[i].points);
        return;
      }
    }
}

int16_t get_points(unsigned long card_id)
{
    for(int i = 0; i < MAX_CARD_COUNT; i++)
    {
      if (card_active_mass[i].card_id == card_id)
      {
        return card_active_mass[i].points;
      }
    }
    return 0;
}

int16_t get_position_points()
{
  int16_t points = get_points(get_card_now_id());
  int16_t result = 1;
  int8_t class_num = get_card_now_class();
  File myFile;
  int i = 0;
  int j = 0;
  int card_num = 0;
  char file_str[60];
  char temp;
  String filename = "points.txt";
  
  card_active_count = 0;
  myFile = SD.open(filename);
  if (myFile) 
  {
    while (myFile.available()) 
    {
      temp = myFile.read();
      if(temp != '$')
      {
        if(temp != '\n')
        {
          file_str[i] = temp;
        }
        i++;
      }
      else
      {
        file_str[i] = '\0';
        temp = myFile.read();
        char card_id[50];
        char point_str[8];
        int8_t class_num;
        int point;
        int t = 0;
        //Serial.println("file_str=" + String(file_str));
        //question_temp[questions_number] = String(questions_str);
        while(file_str[j] != '#')
        {
          card_id[t] = file_str[j];
          t++;
          j++;
        }
        card_id[t] = '\0';
        j++;
        t = 0;
        while(file_str[j] != '#')
        {
          point_str[t] = file_str[j];
          t++;
          j++;
        }
        point_str[t] = '\0';
        point = String(point_str).toInt();

        j++;
        class_num = (int)file_str[j]-48;
        Serial.print("ID ");Serial.print(String(card_id));Serial.print(" points ");Serial.print(point);Serial.print(" class ");Serial.print(class_num);Serial.println("");
        if (class_num == class_num)
        {
          if (points < get_points(strtoul(card_id,NULL,10)))
          {
            result++;
          }
        }
        i = 0;
        j = 0;
      }
    }
    // close the file:
    myFile.close();
  } 
  else 
  {
    // if the file didn't open, print an error:
//    Serial.println("error opening " + filename);
//    for(int i = 0; i < card_count; i++) 
//    {
//        card_dict[i].points = 0;
//    }
  }
  return result;
}

