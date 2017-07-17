/*
     Файл:    permutation_tree_to_permutation.cpp
     Создано: 17 июля 2017г.
     Автор:   Гаврилов Владимир Сергеевич
     E-mails: vladimir.s.gavrilov@gmail.com
              gavrilov.vladimir.s@mail.ru
              gavvs1977@yandex.ru
*/
#include "permutation_tree_to_permutation.h"

const size_t root = 1;
/*
 * The following function uses the centered algorithm of the tree traversal from
 * Корнеев Г.А., Шамгунов Н.Н., Шалыто А.А. Обход дерева на основе автоматного подхода //
 * Компьютерные инструменты в образовании. --- 2004. №3. --- с.32--37.
*/
Permutation permutation_tree_to_permutation(const Permutation_tree& pt){
    Permutation result = Permutation(pt.size() - 1);

    enum class State {Start, Left, Right, Parent};

    State  state         = State::Start;
    size_t writing_idx   = 0;
    size_t curr_node_idx = root;

    do{
        size_t next_node_idx;
        auto curr_node = pt[curr_node_idx];
        switch (state) {
            case State::Start:
                state = State::Left;
                break;
            case State::Left:
                next_node_idx = curr_node.left;
                if(next_node_idx){
                    curr_node_idx = next_node_idx;
                    state = State::Left;
                }else{
                    state = State::Right;
                }
                break;
            case State::Right:
                result[writing_idx++] = curr_node.index - 1;
                next_node_idx         = curr_node.right;
                if(next_node_idx){
                    curr_node_idx = next_node_idx;
                    state = State::Left;
                }else{
                    state = State::Parent;
                }
                break;
            case State::Parent:
                next_node_idx = curr_node.parent;
                if(next_node_idx){
                    state = (pt[next_node_idx].left == curr_node_idx) ? State::Right : State::Parent;
                    curr_node_idx = next_node_idx;
                }else{
                    state = State::Start;
                }
                break;
        }
    }while(state != State::Start);
    return result;
}