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
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <iomanip>
#include "char_conv.h"
#include "map_as_vector.h"
#include "segment.h"
#include "group_pairs.h"
#include "create_permutation_tree.h"
#include "permutation_tree_to_permutation.h"
#include "permutation.h"
#include "create_permutation.h"
#include "myconcepts.h"
#include "list_to_columns.h"

enum Category : uint16_t {
    Spaces,            Other,             Action_name_begin,
    Action_name_body,  Delimiters,        Dollar,
    Backslash,         Opened_square_br,  After_colon,
    After_backslash,   Begin_expr,        End_expr,
    Hat
};

static const char32_t* action_name_begin_chars =
    U"_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char32_t* action_name_body_chars =
    U"_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
static const char32_t* delimiters_chars = U"{}()|*+?";
static const char32_t* after_colon_chars = U"LRbdlnorx";
static const char32_t* after_backslash_chars = U"^\"(){}[]n$|*+\?\\";

std::map<char32_t, uint16_t> table;

static void insert_char(const char32_t ch, Category category){
    auto it = table.find(ch);
    if(it != table.end()){
        table[ch] |= 1U << category;
    }else{
        table[ch] =  1U << category;
    }
}

static void add_category(const char32_t* p, Category category){
    while(char32_t ch = *p++){
        insert_char(ch, category);
    }
}

static std::u32string spaces_str(){
    std::u32string s;
    for(char32_t c = 1; c <= U' '; c++){
        s += c;
    }
    return s;
}

static void fill_table(){
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

template<RandomAccessIterator DestIt, RandomAccessIterator SrcIt, Callable F>
void permutate(DestIt dest_begin, SrcIt src_begin, SrcIt src_end, F f){
    size_t num_of_elems = src_end - src_begin;
    for(size_t i = 0; i < num_of_elems; ++i){
        dest_begin[f(i)] = src_begin[i];
    }
}

template<Integral K, typename V>
SegmentsV<K, V> create_classification_table(const std::map<K, V>& m){
   SegmentsV<K,V> grouped_pairs = group_pairs(map_as_vector(m)); // map_as_vector работает нормально, а также и group_pairs
   size_t         n             = grouped_pairs.size();
   auto           result        = SegmentsV<K, V>(n);
   auto           perm          = create_permutation(grouped_pairs.size());
   auto           f             = [perm](size_t i) -> size_t{return perm[i];};
   permutate(result.begin(), grouped_pairs.begin(), grouped_pairs.end(), f);
   return result;
}

std::string show_char32(char32_t c){
    std::ostringstream oss;
    if(c <= U' '){
        oss << std::setw(4) << static_cast<uint32_t>(c);
    }else if(c == U'\\'){
        oss << R"~(U'\\')~";
    }else{
        oss << "U'" << char32_to_utf8(c) << "'";
    }
    return oss.str();
}

std::string show_table_elem(const Segment_with_value<char32_t, uint16_t> e){
    std::ostringstream oss;
    oss << "{{";
    auto     bounds = e.bounds;
    uint16_t val    = e.value;

    oss << show_char32(bounds.lower_bound) << ", " << show_char32(bounds.upper_bound) << "}, ";
    oss << std::setw(4) << val << "}";
    return oss.str();
}

static const std::string enum_def = R"~(enum Category : uint16_t {
    Spaces,            Other,             Action_name_begin,
    Action_name_body,  Delimiters,        Dollar,
    Backslash,         Opened_square_br,  After_colon,
    After_backslash,   Begin_expr,        End_expr,
    Hat
};

)~";

static const std::string templates = R"~(/*
 * It happens that in std::map<K,V> the key type is integer, and a lot of keys with the same corresponding values.
 * If such a map must be a generated constant, then this map can be optimized. Namely, iterating through a map using
 * range-based for, we will build a std::vector<std::pair<K, V>>.
 * Then we group pairs std::pair<K, V> in pairs in the form (segment, a value of type V), where 'segment' is a struct
 * consisting of lower bound and upper bound. Next, we permute the grouped pair in the such way that in order to search
 * for in the array of the resulting values we can use the algorithm from the answer to exercise 6.2.24 of the book
 * Knuth D.E. The art of computer programming. Volume 3. Sorting and search. --- 2nd ed. --- Addison-Wesley, 1998.
*/

#define RandomAccessIterator typename
#define Callable             typename
#define Integral             typename
template<typename T>
struct Segment{
    T lower_bound;
    T upper_bound;

    Segment()               = default;
    Segment(const Segment&) = default;
    ~Segment()              = default;
};

template<typename T, typename V>
struct Segment_with_value{
    Segment<T> bounds;
    V          value;

    Segment_with_value()                          = default;
    Segment_with_value(const Segment_with_value&) = default;
    ~Segment_with_value()                         = default;
};

/* This function uses algorithm from the answer to the exercise 6.2.24 of the monography
 *  Knuth D.E. The art of computer programming. Volume 3. Sorting and search. --- 2nd ed.
 *  --- Addison-Wesley, 1998.
*/
template<RandomAccessIterator I, typename K>
std::pair<bool, size_t> knuth_find(I it_begin, I it_end, K key)
{
    std::pair<bool, size_t> result = {false, 0};
    size_t                  i      = 1;
    size_t                  n      = it_end - it_begin;
    while (i <= n) {
        const auto& curr        = it_begin[i - 1];
        const auto& curr_bounds = curr.bounds;
        if(key < curr_bounds.lower_bound){
            i = 2 * i;
        }else if(key > curr_bounds.upper_bound){
            i = 2 * i + 1;
        }else{
            result.first = true; result.second = i - 1;
            break;
        }
    }
    return result;
}
)~";

static const std::string categories_table_top =
    "static const Segment_with_value<char32_t, uint64_t> categories_table[] = {\n";

static std::string size_const(size_t n){
    std::string result;
    result = "static const size_t num_of_elems_in_categories_table = " +
             std::to_string(n) + ";\n\n";
    return result;
}

static const std::string get_categories_set_func =
    R"~(uint64_t get_categories_set(char32_t c){
    auto t = knuth_find(categories_table,
                        categories_table + num_of_elems_in_categories_table,
                        c);

    return t.first ? categories_table[t.second].value : (1ULL << Other)
}
)~";

std::string show_table(){
    std::string s = enum_def + templates + categories_table_top;

    auto        t = create_classification_table(table);

    Format      f;
    f.indent                 = 4;
    f.number_of_columns      = 3;
    f.spaces_between_columns = 2;

    std::vector<std::string> elems;

    size_t num_of_elems   = t.size();

    for(const auto& e : t){
        elems.push_back(show_table_elem(e));
    }

    s += string_list_to_columns(elems, f) + "\n};\n\n";
    s += size_const(num_of_elems) + get_categories_set_func;
    return s;
}

void print(){
    std::string s = show_table();
    printf("%s\n", s.c_str());
}

// #define DEBUG
#ifdef DEBUG
void print_filled_table(){
    for(const auto e : table){
        std::string s = show_char32(e.first);
        printf("{%s, %d} \n", s.c_str(), e.second);
    }
    putchar('\n');
}

void print_vector(const std::vector<std::pair<char32_t, uint16_t>>& v){
    for(const auto e : v){
        std::string s = show_char32(e.first);
        printf("{%s, %d} \n", s.c_str(), e.second);
    }
    putchar('\n');
}

void print_grouped_vector(const SegmentsV<char32_t, uint16_t>& gv){
    for(const auto e : gv){
        auto s = show_table_elem(e);
        printf("%s \n",s.c_str());
    }
    putchar('\n');
}

void print_permutation_node(const Permutation_node& node){
    printf("{index = %zu, left = %zu, right = %zu, parent = %zu}",
           node.index, node.left, node.right, node.parent);
}

void print_permutation_tree(const Permutation_tree& pt){
    for(const auto& node : pt){
        print_permutation_node(node);
        putchar('\n');
    }
}

void print_permutation(const Permutation& p){
    for(auto i : p){
        printf("%zu ", i);
    }
    putchar('\n');
}
#endif

int main(){
    fill_table();
#ifdef DEBUG
    puts("Table as map:");
    print_filled_table();
    puts("*******************************************************************");
    auto v  = map_as_vector(table);
    puts("Table as vector:");
    print_vector(v);
    puts("*******************************************************************");
    printf("Number of elements in vector is: %zu.\n", v.size());
    puts("*******************************************************************");
    auto gv = group_pairs(v);
    puts("Grouped pairs: ");
    print_grouped_vector(gv);
    puts("*******************************************************************");
    printf("Number of grouped pairs is: %zu.\n", gv.size());
    puts("*******************************************************************");
    puts("Permutation tree:");
    auto pt = create_permutation_tree(gv.size());
    print_permutation_tree(pt);
    puts("*******************************************************************");
    auto p = permutation_tree_to_permutation(pt);
    puts("Permutation: ");
    print_permutation(p);
    puts("*******************************************************************");
    puts("Result of function create_permutation:");
    auto cp = create_permutation(gv.size());
    print_permutation(cp);
    puts("*******************************************************************");
    auto t = create_classification_table(table);
    puts("Final classification table is: ");
    print_grouped_vector(t);
    puts("*******************************************************************");
    print();
#else
    print();
#endif
    return 0;
}