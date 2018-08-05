/*!
 *  \file      sd_card_module.h
 *
 *  \brief     Функции для работы с SD картой
 *
 *  \detailed  
 *
 *  \author    Андрей Алексеев <alekseev@thinking-house.ru
 *
 *  \copyright Лаборатория Умного Дома (ИП Алексеев А.П.), 2018. Все права защищены.
 */
#ifndef _SD_CARD_MODULE_H_
#define _SD_CARD_MODULE_H_

//! Подключение заголовочных файлов
#include "Arduino.h"
#include "common.h"
#include <UTFT_SdRaw.h>                           // подключаем библиотеку UTFT_SdRaw     для вывода изображений с SD-карты на дисплей

//! Глобальные данные


//! Макроопределения
#define CARD_BLOCK_TIME 5400
#define MAX_TIME_FOR_ANSWER 20


//! Типы
typedef struct
{
  char text[32];
  bool is_true;
}answer_t;

typedef struct
{
  char text[255];
  answer_t answers[4];
}question_t;

//! Типы
typedef struct 
{ 
  unsigned long card;
  String name;
  int8_t class_num;
} card_t;

typedef struct
{
  unsigned long card_id;
  int16_t block_time;
  int16_t points;
}active_card_t;

//! Глобальные функции
/*!
 *  \brief         Инициализация SD карты
 *
 */
void sd_card_init();

/*!
 *  \brief         Получить вопрос из SD
 *
 *  \param  [in]   class_number - Класс 
 *  \param  [in]   num номер по порядку
 *  
 *  \return        Структура с вопросом и ответами
 */
question_t get_question(int8_t class_number, int num);

/*!
 *  \brief         Посчитать вопросы на флешке 
 *
 *  \param  [in]   class_number - Класс 
 *  
 *  \return        количество
 */
int count_question(int class_number);

/*!
 *  \brief         Получить карты из файла
 *  
 *  \return        Результат успешено или нет
 */
bool read_cards_from_file();

/*!
 *  \brief         Проверка блокирована карта или нет
 *  
 *  \return        Время до разблокировки
 */
int16_t check_card_block(void);

/*!
 *  \brief         Получить ID текущей карты
 *  
 *  \return        ID карты
 */
unsigned long get_card_now_id(void);
/*!
 *  \brief         Получить калсс текущей карты
 *  
 *  \return        номер класса
 */
int8_t get_card_now_class(void);

/*!
 *  \brief         Получить ФИО текущей карты
 *  
 *  \return        ID карты
 */
String get_card_now_fio(void);

/*!
 *  \brief         Блокировать текущую карту
 */
void set_card_block(void);

/*!
 *  \brief         Уменьшить время блокиовки карт
 *  
 *  \param         time_n - Время уменьшения блокировки
 */
void down_card_block(int time_n);

/*!
 *  \brief         Установить ID текущей карты
 *  
 *  \param         ID карты
 */
void set_card_now_id(unsigned long card_id);

/*!
 *  \brief         Сбросить ID текущей карты
 */
void reset_card_now(void);

/*!
 *  \brief         Прочитать очки из файла
 */
void read_point_from_file(void);

/*!
 *  \brief         Сохранить очки в файл
 */
void save_points_to_file(void);

/*!
 *  \brief         Добавить очко текущей карте
 */
void add_point();

/*!
 *  \brief         Получить позицию в рейтинге
 *  
 *  \return        Позиция
 */
int16_t get_position_points(void);

#endif
