/*
    File:    list_to_columns.cpp
    Created: 06 November 2016 at 12:20 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#include "list_to_columns.h"
#include <algorithm>

std::vector<size_t> calculate_columns_width(size_t num_of_columns,
                                            const std::vector<std::string>& l)
{
    std::vector<size_t> result = std::vector<size_t>(num_of_columns);
    for(size_t& z : result){
        z = 0;
    }
    size_t counter = 0;
    for(const auto& s : l){
        size_t i = counter % num_of_columns;
        result[i] = std::max(result[i], s.length());
        counter++;
    }
    return result;
}

std::string string_list_to_columns(const std::vector<std::string>& l, const Format& f,
                                   char  d)
{
    std::string result;
    size_t      num_of_strs    = l.size();
    size_t      num_of_columns = f.number_of_columns;

    if(!num_of_strs || (num_of_columns <= 0)){
        return result;
    }

    num_of_columns = std::min(num_of_columns, num_of_strs);
//     if(num_of_columns > num_of_strs){
//         num_of_columns = num_of_strs;
//     }
    size_t     num_of_rows    = num_of_strs / num_of_columns;
    size_t     rest           = num_of_strs % num_of_columns;

    using Row = std::vector<std::string>;

    std::string delim;
    size_t      delim_len = 0;
    if(d){
        delim     += d;
        delim_len =  1;
    }

    auto        begin_indent   = std::string(f.indent, ' ');
    auto        interm_indent  = std::string(f.spaces_between_columns, ' ');
    auto        column_width   = calculate_columns_width(num_of_columns, l);
    size_t delim_len_and_interm_indent = delim_len + f.spaces_between_columns + 1;
    size_t num_of_deleted_char = 0;
    for(size_t i = 0; i < num_of_rows; i++){
        Row    current_row;
        size_t num_of_padded_spaces;
        for(size_t j = 0; j < num_of_columns; j++){
            auto current_line = l[num_of_columns * i + j];
            num_of_padded_spaces = column_width[j] - current_line.length();
            auto temp = current_line + delim + std::string(num_of_padded_spaces, ' ');
            current_row.push_back(temp);
        }
        result += begin_indent;
        for(const auto e : current_row){
            result += e + interm_indent;
        }
        result += '\n';
        num_of_deleted_char = num_of_padded_spaces + delim_len_and_interm_indent;
    }
    if(rest){
        Row    current_row;
        size_t num_of_padded_spaces = 0;
        for(size_t i = num_of_strs - rest; i < num_of_strs; i++){
            auto current_line = l[i];
            num_of_padded_spaces = column_width[i + rest - num_of_strs] - current_line.length();
            auto temp = current_line + delim + std::string(num_of_padded_spaces, ' ');
            current_row.push_back(temp);
        }
        result += begin_indent;
        for(const auto e : current_row){
            result += e + interm_indent;
        }
        result += '\n';
        num_of_deleted_char = num_of_padded_spaces + delim_len_and_interm_indent;
    }
    result.erase(result.end() - num_of_deleted_char, result.end());
    return result;
}