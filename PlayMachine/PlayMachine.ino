/*!
 *  \file      MAIN
 *
 *  \brief     
 *
 *  \detailed  
 *
 *  \author    Андрей Алексеев <alekseev@thinking-house.ru
 *
 *  \copyright Лаборатория Умного Дома (ИП Алексеев А.П.), 2018. Все права защищены.
 */

//! Подключение заголовочных файлов
#include "pitches.h"
#include "sd_card_module.h" 
#include "rfid_module.h"
#include "display.h"
#include "common.h"

//! Макроопределения
#define BTN1 A12
#define BTN2 A13
#define BTN3 A14
#define BTN4 A15

// Определение пинов для управления двигателем
#define IN1  A11 // IN1 на 1-м драйвере ULN2003
#define IN2  A10 // IN2 на 1-м драйвере ULN2003
#define IN3  A9 // IN3 на 1-м драйвере ULN2003
#define IN4  A8 // IN4 на 1-м драйвере ULN2003
#define DELAY_STEP 1
#define STEP_COUNT 256

#define SNAKE_ON 3 

//! Локальные данные                        //
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

int questions_number = 0;
unsigned long msecond;
boolean is_image_draw = false;

//! Локальные функции
void play_melody()
{
  for (int thisNote = 0; thisNote < 8; thisNote++) 
  {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(44, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(44);
  }
}

void play_melody_start()
{
    tone(44, NOTE_C4, 400);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = 1000 * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(44);
}

void stepForward()
{
    digitalWrite( IN1, LOW ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, HIGH ); 
    digitalWrite( IN4, HIGH ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, LOW ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, HIGH ); 
    digitalWrite( IN4, LOW ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, LOW ); 
    digitalWrite( IN2, HIGH ); 
    digitalWrite( IN3, HIGH ); 
    digitalWrite( IN4, LOW ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, HIGH ); 
    digitalWrite( IN2, HIGH ); 
    digitalWrite( IN3, LOW ); 
    digitalWrite( IN4, LOW ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, HIGH ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, LOW ); 
    digitalWrite( IN4, LOW ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, HIGH ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, LOW ); 
    digitalWrite( IN4, HIGH ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, LOW ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, LOW ); 
    digitalWrite( IN4, HIGH ); 
    delay(DELAY_STEP);
}

void stepBack()
{
    digitalWrite( IN1, LOW ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, LOW ); 
    digitalWrite( IN4, HIGH ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, HIGH ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, LOW ); 
    digitalWrite( IN4, HIGH ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, HIGH ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, LOW ); 
    digitalWrite( IN4, LOW ); 
    delay(DELAY_STEP);
    
    digitalWrite( IN1, HIGH ); 
    digitalWrite( IN2, HIGH ); 
    digitalWrite( IN3, LOW ); 
    digitalWrite( IN4, LOW ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, LOW ); 
    digitalWrite( IN2, HIGH ); 
    digitalWrite( IN3, HIGH ); 
    digitalWrite( IN4, LOW ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, LOW ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, HIGH ); 
    digitalWrite( IN4, LOW ); 
    delay(DELAY_STEP); 
    
    digitalWrite( IN1, LOW ); 
    digitalWrite( IN2, LOW ); 
    digitalWrite( IN3, HIGH ); 
    digitalWrite( IN4, HIGH ); 
    delay(DELAY_STEP); 
}

void rotationForward(int steps)
{
  for(int i = 0; i < steps; i++)
    stepForward();
}

void rotationBack(int steps)
{
  for(int i = 0; i < steps; i++)
    stepBack();
}

void setup() {
  randomSeed(analogRead(A8));
//---Включение UaRT----------------------
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
//---Играем мелодию
  play_melody_start();
//----------------------------------------

//---Инициализируем кнопочки
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);
  pinMode(BTN4, INPUT);
//---------------------------------------

//---Пины мотора-----------------------
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
//---------------------------------------- 

//---Инициализация SPI
  SPI.begin();        // Init SPI bus
//----------------------------------------
//---Светодиод о начале игры
  pinMode(24, OUTPUT);
  pinMode(SNAKE_ON, OUTPUT);
  digitalWrite(SNAKE_ON, LOW);
//----
delay(100);
//---Инициализация SD карты
  sd_card_init();

//---Инициализация RFID
  rfid_init();

//---
  read_point_from_file();
  init_display();
  delay(500);
  msecond = millis();
  print_log("START PROGRAMM");
}

void loop() 
{
  digitalWrite(SNAKE_ON, LOW);
  delay(1000);
  int temp_number;
  int16_t question_numbers[5];
  int time_down = (millis() - msecond) / 1000;
  int16_t block_time;
  int questions_count;
  int8_t true_answer;
  
  down_card_block(time_down);
  msecond = millis();
  
  //---Рисуем изображение, если надо------------------------------------------------------------
  if(!is_image_draw)
    show_bmp();
  is_image_draw = true;
  delay(500);
  
  //---Читаем приложенную карту------------------------------------------------------------------
  set_card_now_id(wait_card_id());
  if (get_card_now_id() == 0)
    return;

  //---Ищем приложенную карту в файле
  if (read_cards_from_file() == false)
  {
    if (get_card_now_id() != 0)
    {
      print_card_info(get_card_now_id());
      delay(5000);
      is_image_draw = false;
      wait_btn(-1);
    }
    return;
  }

  //---Получаем данные карты---------------------------------------------------------------------

  block_time = check_card_block();
  print_log("Приложена карта");
  print_log("ID " + String(get_card_now_id()) + " class " + String(get_card_now_class()) + " FIO " + String(get_card_now_fio()) + " block time "+ String(block_time));

  if (block_time > 0)
  {
    print_points(block_time);
    delay(5000);
    is_image_draw = false;
    return;
  }
  delay(1000);

  //---Получаем вопросы---------------------------------------------------------------------------
  questions_count = count_question(get_card_now_class());
  if (questions_count < 5)
  {
    Serial.println("Мало Вопросов");
    print_message("Questions for class " + String(get_card_now_class()) + " < 5");
    delay(5000);
    is_image_draw = false;
  }
  //1
  question_numbers[0] = random(0, questions_count);
  //2
  while(1)
  {
    temp_number = random(0, questions_count);
    if(temp_number != question_numbers[0])
    {
      question_numbers[1] = temp_number;
      break;
    }
  }
  //3
  while(1)
  {
    temp_number = random(0, questions_count);
    if(temp_number != question_numbers[0] && temp_number != question_numbers[1])
    {
      question_numbers[2] = temp_number;
      break;
    }
  }
  //4
  while(1)
  {
    temp_number = random(0, questions_count);
    if(temp_number != question_numbers[0] && temp_number != question_numbers[1] && temp_number != question_numbers[2])
    {
      question_numbers[3] = temp_number;
      break;
    }
  }
  //5
  while(1)
  {
    temp_number = random(0, questions_count);
    if(temp_number != question_numbers[0] && temp_number != question_numbers[1] && temp_number != question_numbers[2] && temp_number != question_numbers[3])
    {
      question_numbers[4] = temp_number;
      break;
    }
  }

  Serial.println("Сгенерированные номера:" + String(question_numbers[0]) + "," + String(question_numbers[1]) + "," + String(question_numbers[2]) + "," + String(question_numbers[3]) + "," + String(question_numbers[4]));
  delay(500);
  is_image_draw = false;

  //---Game Start--------------------------------------------------------------
  play_melody_start();
  Serial.println("Game Start");
  digitalWrite(24, HIGH);
  digitalWrite(SNAKE_ON, HIGH);
  print_message("Good Luck " + String(get_card_now_fio()));
  delay(5000);
  set_card_block();
  for(int i = 0; i < 5; i++)
  {
      question_t question = get_question(get_card_now_class(), question_numbers[i]);
      
      true_answer = print_question(question);
      if(true_answer != wait_btn(MAX_TIME_FOR_ANSWER * 1000))
      {
        print_message("Game Over");
        digitalWrite(24, LOW);
        digitalWrite(SNAKE_ON, LOW);
        delay(15000);
        return;
      }
      print_message("True. Next question");
      delay(5000);
  }
  show_win();
  play_melody();
  add_point();
  save_points_to_file();
  reset_card_now();
  rotationForward(STEP_COUNT);
  delay(5000);
  rotationBack(STEP_COUNT);
  digitalWrite(24, LOW);
  digitalWrite(SNAKE_ON, LOW);
  delay(15000);
}

int wait_btn(int timeout)
{
  int flag_exit=0;
  while(flag_exit < timeout || timeout == -1)
  {
    if(digitalRead(BTN1) == 0)
    {
      Serial.println("Нажата кнопка 1");
      return 1;
    }
    if(digitalRead(BTN2) == 0)
    {
      Serial.println("Нажата кнопка 2");
      return 2;
    }
    if(digitalRead(BTN3) == 0)
    {
      Serial.println("Нажата кнопка 3");
      return 3;
    }
    if(digitalRead(BTN4) == 0)
    {
      Serial.println("Нажата кнопка 4");
      return 4;
    }
    if (timeout != -1)
      flag_exit++;
    delay(1);
  }
  return 0;
}
