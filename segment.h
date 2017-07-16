/*
     Файл:    segment.h
     Создано: 16 июля 2017г.
     Автор:   Гаврилов Владимир Сергеевич
     E-mails: vladimir.s.gavrilov@gmail.com
              gavrilov.vladimir.s@mail.ru
              gavvs1977@yandex.ru
*/
#ifndef SEGMENT_H
#define SEGMENT_H
template<typename T>
struct Segment{
    T lower_bound;
    T upper_bound;

    Segment(T l, T u) : lower_bound(l), upper_bound(u) {};
    Segment()               = default;
    Segment(const Segment&) = default;
    ~Segment()              = default;
};

template<typename T, typename V>
struct Segment_with_value{
    Segment<T> bounds;
    V          value;

    Segment_with_value(Segment<T> b, V v) : bounds(b), value(v) {};
    Segment_with_value()                          = default;
    Segment_with_value(const Segment_with_value&) = default;
    ~Segment_with_value()                         = default;
};

template<typename K, typename V>
using SegmentsV = std::vector<Segment_with_value<K,V>>;
#endif