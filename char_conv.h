/*   
     Файл:    char_conv.h
     Создано: 28 января 2016г.
     Автор:   Гаврилов Владимир Сергеевич
     E-mails: vladimir.s.gavrilov@gmail.com
              gavrilov.vladimir.s@mail.ru
              gavvs1977@yandex.ru
*/

/**
\file

\brief Заголовочный файл с прототипами функций, преобразующих строки из кодировки
UTF-8 в кодировку UTF-32 и наоборот.
*/

#ifndef CHAR_CONV_H
#define CHAR_CONV_H

#include <string>

/** 
\function utf8_to_u32string 
 Данная функция по строке в кодировке UTF-8 строит строку в кодировке UTF-32.
 
\param  utf8str – строка в кодировке UTF-8 с завершающим нулевым символом  

\return значение типа std::u32string, представляющее собой ту же строку, 
 но в кодировке UTF-32
*/
std::u32string utf8_to_u32string(const char* utf8str);

/** 
\function u32string_to_utf8 
 Данная функция по строке в кодировке UTF-32 строит строку в кодировке UTF-8.
 
\param [in] u32str – строка в кодировке UTF-32

\return значение типа std::string, представляющее собой ту же строку, 
 но в кодировке UTF-8
*/
std::string u32string_to_utf8(const std::u32string& u32str);

/**
\function char32_to_utf8 
По символу в кодировке UTF-32 строит строку, состоящую из байтов, представляющих
тот же символ, но в кодировке UTF-8.

\param [in] с - символ в кодировке UTF-32

\return Значение типа std::string, состоящее из байтов, представляющих 
тот же символ, но в кодировке UTF-8.
*/
std::string char32_to_utf8(const char32_t c);
#endif
