/*
     Файл:    map_as_vector.h
     Создано: 16 июля 2017г.
     Автор:   Гаврилов Владимир Сергеевич
     E-mails: vladimir.s.gavrilov@gmail.com
              gavrilov.vladimir.s@mail.ru
              gavvs1977@yandex.ru
*/
#ifndef MAP_AS_VECTOR_H
#define MAP_AS_VECTOR_H

#include <vector>
#include <map>
#include <utility>

template<typename K, typename V>
std::vector<std::pair<K, V>> map_as_vector(const std::map<K, V>& m){
    std::vector<std::pair<K, V>> result;
    for(const auto e : m){
        result.push_back(e);
    }
    return result;
}

#endif