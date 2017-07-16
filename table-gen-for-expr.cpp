/*
     Файл:    table-gen-for-expr.cpp
     Создано: 30 января 2016г.
     Автор:   Гаврилов Владимир Сергеевич
     E-mails: vladimir.s.gavrilov@gmail.com
              gavrilov.vladimir.s@mail.ru
              gavvs1977@yandex.ru
*/
#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <iomanip>
#include "char_conv.h"

enum Category : uint16_t {
    Spaces,            Other,             Action_name_begin,
    Action_name_body,  Delimiters,        Dollar,
    Backslash,         Opened_square_br,  After_colon,
    After_backslash,   Begin_expr,        End_expr,
    Hat
};

const char32_t* action_name_begin_chars =
    U"_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char32_t* action_name_body_chars =
    U"_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const char32_t* delimiters_chars = U"{}()|*+?";
const char32_t* after_colon_chars = U"LRbdlnorx";
const char32_t* after_backslash_chars = U"(){}[]n$|*+\?\\";

std::map<char32_t, uint16_t> table;

void insert_char(const char32_t ch, Category category){
    auto it = table.find(ch);
    if(it != table.end()){
        table[ch] |= 1U << category;
    }else{
        table[ch] =  1U << category;
    }
}

void add_category(const char32_t* p, Category category){
    while(char32_t ch = *p++){
        insert_char(ch, category);
    }
}

std::u32string spaces_str(){
    std::u32string s;
    for(char32_t c = 1; c <= ' '; c++){
        s += c;
    }
    return s;
}

void fill_table(){
    std::u32string s = spaces_str();

    add_category(s.c_str(), Spaces);
    add_category(action_name_begin_chars, Action_name_begin);
    add_category(action_name_body_chars, Action_name_body);
    add_category(delimiters_chars, Delimiters);
    add_category(after_colon_chars, After_colon);
    add_category(after_backslash_chars, After_backslash);
    add_category(U"$", Dollar);
    add_category(U"[", Opened_square_br);
    add_category(U"\\", Backslash);
    add_category(U"{", Begin_expr);
    add_category(U"}", End_expr);
    add_category(U"^", Hat);
}

std::string show_table_elem(const std::pair<char32_t, uint16_t> e){
    std::ostringstream oss;
    oss << "{";
    auto c = e.first;
    if(c <= ' '){
        oss << std::setw(4) << c;
    }else{
        oss << "U'" << char32_to_utf8(e.first) << "'";
    }
    oss <<  ", " << std::setw(4) << e.second << "}";
    return oss.str();
}

std::string show_table(){
    std::string s = R"~(enum Category : uint16_t {
    Spaces,            Other,             Action_name_begin,
    Action_name_body,  Delimiters,        Dollar,
    Backslash,         Opened_square_br,  After_colon,
    After_backslash,   Begin_expr,        End_expr
};

std::map<char32_t, uint16_t> categories_table = {
)~";
    #define ELEMS_IN_GROUP 4
    size_t num_of_elems   = table.size();
    size_t num_of_triples = num_of_elems / ELEMS_IN_GROUP;
    size_t rem            = num_of_elems % ELEMS_IN_GROUP;
    auto it = table.begin();
    for(size_t i = 0; i < num_of_triples; i++){
        s += "    ";
        for(int j = 0; j < ELEMS_IN_GROUP; j++, it++){
            s += show_table_elem(*it) + ",  ";
        }
        s += "\n";
    }
    s += "    ";
    for(unsigned j = 0; j < rem; j++, it++){
        s += show_table_elem(*it) + ",  ";
    }
    s.pop_back();
    s.pop_back();
    s += "\n};";
    return s;
}
void print(){
    std::string s = show_table();
    printf("%s\n", s.c_str());
}

int main(void) {
    fill_table();
    print();
    return 0;
}