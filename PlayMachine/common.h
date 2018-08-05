/*!
 *  \file      common.h
 *
 *  \brief     Базовые функции
 *
 *  \detailed  
 *
 *  \author    Андрей Алексеев <alekseev@thinking-house.ru
 *
 *  \copyright Лаборатория Умного Дома (ИП Алексеев А.П.), 2018. Все права защищены.
 */
#ifndef _COMMON_H_
#define _COMMON_H_

//! Подключение заголовочных файлов
#include "Arduino.h"
#include <UTFT.h>                                 // подключаем библиотеку UTFT           для работы с дисплеем
#include <UTFT_SdRaw.h>                           // подключаем библиотеку UTFT_SdRaw     для вывода изображений с SD-карты на дисплей
#include <SdFat.h>

//! Макроопределения


//! Глобальные функции
void print_log(String text);
#endif
