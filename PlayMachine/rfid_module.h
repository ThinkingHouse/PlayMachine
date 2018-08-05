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
#include "common.h"

//! Макроопределения

//! Глобальные функции
/*!
 *  \brief         Инициализация модуля RFID
 *
 */
void rfid_init(void);

/*!
 *  \brief         Получить ID приложенной карты
 *  
 *  \param         ID карты
 */
unsigned long wait_card_id(void);

