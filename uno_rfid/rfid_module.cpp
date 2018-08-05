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
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

//! Локальные данные 
MFRC522 mfrc522(SS_PIN, RST_PIN);

//! Глобальные функции
void rfid_init()
{
  mfrc522.PCD_Init();     // инициализация MFRC522 / Init MFRC522 card.
}

unsigned long get_card_id()
{
  unsigned long uidDec, uidDecTemp;  // для храниения номера метки в десятичном формате
    // Поиск новой метки
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  // Выбор метки
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  uidDec = 0;
  // Выдача серийного номера метки.
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }
  //Serial.println("Card UID: ");
  //Serial.println(String(uidDec)); // Выводим UID метки в консоль.
  return uidDec;
}

