/*
    Copyright (c) 2020 Ivan Kniazkov <ivan.kniazkov.com>

    The implementation of the strings library
*/

#include "strings.h"
#include "allocator.h"

string_builder_t * create_string_builder(size_t capacity)
{
    size_t size = sizeof(string_builder_t) + sizeof(char) * (capacity + 1);
    string_builder_t *obj = nnalloc(size);
    obj->data = (char*)(obj + 1);
    obj->data[0] = '\0';
    obj->length = 0;
    obj->capacity = capacity;
    return obj;
}

string_builder_t * append_char(string_builder_t *obj, char ch)
{
    if (obj->capacity > obj->length)
    {
        obj->data[obj->length++] = ch;
        obj->data[obj->length] = '\0';
        return obj;
    }
    else
    {
        size_t new_capacity = obj->capacity ? obj->capacity * 2 : 16;
        size_t size = sizeof(string_builder_t) + sizeof(char) * (new_capacity + 1);
        string_builder_t *new_obj = nnalloc(size);
        new_obj->length = obj->length + 1;
        new_obj->capacity = new_capacity;
        new_obj->data = (char*)(new_obj + 1);
        memcpy(new_obj->data, obj->data, obj->length);
        new_obj->data[obj->length] = ch;
        new_obj->data[new_obj->length] = '\0';
        free(obj);
        return new_obj;
    }
}