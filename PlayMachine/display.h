/*!
 *  \file      display.h
 *
 *  \brief     Функции для работы с дисплеем
 *
 *  \detailed  
 *
 *  \author    Андрей Алексеев <alekseev@thinking-house.ru
 *
 *  \copyright Лаборатория Умного Дома (ИП Алексеев А.П.), 2018. Все права защищены.
 */
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//! Подключение заголовочных файлов
#include "Arduino.h"
#include <UTFT.h>                                 // подключаем библиотеку UTFT           для работы с дисплеем
#include <UTFT_SdRaw.h>                           // подключаем библиотеку UTFT_SdRaw     для вывода изображений с SD-карты на дисплей
#include "sd_card_module.h" 
#include "Grotesk.h"

//! Макроопределения

//Цвета
#define QUESTION_TEXT_COLOR VGA_BLUE
#define ANSWER_TEXT_COLOR   VGA_BLUE
#define SERVICE_MESSAGE_COLOR VGA_RED

//Координаты строк
#define QUESTION_TEXT_X 5
#define QUESTION_TEXT_Y 16

#define ANSWER1_TEXT_X 5
#define ANSWER1_TEXT_Y 268

#define ANSWER2_TEXT_X 5
#define ANSWER2_TEXT_Y 321

#define ANSWER3_TEXT_X 5
#define ANSWER3_TEXT_Y 374

#define ANSWER4_TEXT_X 5
#define ANSWER4_TEXT_Y 427
// Assign human-readable names to some common 16-bit color values:

//! Типы

//! Глобальные функции
/*!
 *  \brief         Инициализация дисплея
 *
 */
void init_display(void);

/*!
 *  \brief         Вывести картинку на экран
 *
 */
void show_bmp(void);

/*!
 *  \brief         Вывести информацию о карте на экран
 *  
 *  \param         card_id - ID карты
 *
 */
void print_card_info(unsigned long card_id);

/*!
 *  \brief         Вывести строку на экран
 *  
 *  \param         mess - Строка
 *
 */
void print_message(String mess);

/*!
 *  \brief         Вывести вопрос на экран
 *  
 *  \param         question - Структура с параметрами вопроса
 *
 */
int print_question(question_t question);

/*!
 *  \brief         Вывести очки и время до разблокировки карты на экран
 *  
 *  \param         block_time - оставшейся время
 *
 */
void print_points(int block_time);

/*!
 *  \brief         Вывести сообщение о победе на экран
 */
void show_win(void);

#endif
