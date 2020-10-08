/*
    Copyright (c) 2020 Ivan Kniazkov <ivan.kniazkov.com>

    The implementation of the strings library
*/

#include "strings.h"
#include "allocator.h"

string_builder_t * create_string_builder(size_t capacity)
{
    size_t size = sizeof(string_builder_t) + sizeof(char) * (capacity + 1);
    string_builder_t *this = nnalloc(size);
    this->data = (char*)(this + 1);
    this->data[0] = '\0';
    this->length = 0;
    this->capacity = capacity;
    return this;
}
