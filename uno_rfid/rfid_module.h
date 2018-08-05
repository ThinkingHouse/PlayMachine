/*!
 *  \file      rfid_module.h
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
#include "Arduino.h"
#include <SPI.h>
#include <MFRC522.h> // библиотека "RFID".

//! Макроопределения

//! Глобальные функции
void rfid_init(void);
unsigned long get_card_id(void);

