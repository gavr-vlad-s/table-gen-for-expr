/*
     Файл:    create_permutation_tree.cpp
     Создано: 17 июля 2017г.
     Автор:   Гаврилов Владимир Сергеевич
     E-mails: vladimir.s.gavrilov@gmail.com
              gavrilov.vladimir.s@mail.ru
              gavvs1977@yandex.ru
*/
#include "create_permutation_tree.h"

void create_permutation_treeR(Permutation_tree& p, size_t n, size_t node_idx, size_t parent_idx)
{
    if(node_idx > n){return;}

    if(node_idx & 1){
        p[parent_idx].right = node_idx;
    }else{
        p[parent_idx].left  = node_idx;
    }

    p[node_idx].index  = node_idx;
    p[node_idx].parent = parent_idx;

    create_permutation_treeR(p, n, 2 * node_idx,     node_idx);
    create_permutation_treeR(p, n, 2 * node_idx + 1, node_idx);
}

Permutation_tree create_permutation_tree(size_t n){
    Permutation_tree p = Permutation_tree(n + 1);
    p[1].index = 1;
    create_permutation_treeR(p, n, 2, 1);
    create_permutation_treeR(p, n, 3, 1);
    return p;
}