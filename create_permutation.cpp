/*
     Файл:    create_permutation.cpp
     Создано: 17 июля 2017г.
     Автор:   Гаврилов Владимир Сергеевич
     E-mails: vladimir.s.gavrilov@gmail.com
              gavrilov.vladimir.s@mail.ru
              gavvs1977@yandex.ru
*/
#include "create_permutation.h"
#include "create_permutation_tree.h"
#include "permutation_tree_to_permutation.h"

Permutation create_permutation(size_t n){
    Permutation result;
    Permutation_tree pt = create_permutation_tree(n);
    result              = permutation_tree_to_permutation(pt);
    return result;
}