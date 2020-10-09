/*
    Copyright (c) 2020 Ivan Kniazkov <ivan.kniazkov.com>

    The implementation of the strings library
*/

#include "strings.h"
#include "allocator.h"
#include <stdarg.h>

static const size_t initial_capacity = 16;

static __inline string_builder_t * instantiate_string_builder(size_t capacity)
{
    size_t size = sizeof(string_builder_t) + sizeof(char) * (capacity + 1);
    string_builder_t *obj = nnalloc(size);
    obj->data = (char*)(obj + 1);
    obj->capacity = capacity;
    return obj;
}

string_builder_t * create_string_builder(size_t capacity)
{
    string_builder_t *obj = instantiate_string_builder(capacity);
    obj->data[0] = '\0';
    obj->length = 0;
    return obj;
}

string_builder_t * append_char(string_builder_t *obj, char ch)
{
    if (!obj)
    {
        obj = instantiate_string_builder(initial_capacity);
        obj->data[0] = ch;
        obj->data[1] = '\0';
        obj->length = 1;
        return obj;
    }
    else if (obj->capacity > obj->length)
    {
        obj->data[obj->length++] = ch;
        obj->data[obj->length] = '\0';
        return obj;
    }
    else
    {
        size_t new_capacity = obj->capacity ? obj->capacity * 2 : initial_capacity;
        string_builder_t *new_obj = instantiate_string_builder(new_capacity);
        new_obj->length = obj->length + 1;
        memcpy(new_obj->data, obj->data, obj->length * sizeof(char));
        new_obj->data[obj->length] = ch;
        new_obj->data[new_obj->length] = '\0';
        free(obj);
        return new_obj;
    }
}

string_builder_t * append_string(string_builder_t *obj, string_t str)
{
    if (!obj)
    {
        size_t capacity = initial_capacity;
        if (capacity < str.length) capacity = str.length;
        obj = instantiate_string_builder(capacity);
        memcpy(obj->data, str.data, str.length * sizeof(char));
        obj->data[str.length] = '\0';
        obj->length = str.length;
        return obj;
    }
    size_t new_length = obj->length + str.length;
    if (new_length <= obj->capacity)
    {
        memcpy(obj->data + obj->length, str.data, str.length * sizeof(char));
        obj->data[new_length] = '\0';
        obj->length = new_length;
        return obj;
    }
    else
    {
        size_t new_capacity = obj->capacity ? obj->capacity * 2 : initial_capacity;
        if (new_capacity < new_length) new_capacity = new_length;
        string_builder_t *new_obj = instantiate_string_builder(new_capacity);
        new_obj->length = new_length;
        memcpy(new_obj->data, obj->data, obj->length * sizeof(char));
        memcpy(new_obj->data + obj->length, str.data, str.length * sizeof(char));
        new_obj->data[new_obj->length] = '\0';
        free(obj);
        return new_obj;
    }    
}

string_builder_t * append_formatted_string(string_builder_t *obj, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    const char *ptr = format;
    while(*ptr)
    {
        if (*ptr == '%')
        {
            ptr++;
            switch(*ptr)
            {
                case 's':
                {
                    const char *c_str = va_arg(va, const char*);
                    obj = append_string(obj, _S(c_str));
                    break;
                }

                default:
                    obj = append_char(obj, *ptr);
                    break;
                    
                case 0:
                    obj->data[0] = '\0';
                    obj->length = 0;
                    return obj;
            }
            ptr++;
        }
        else
        {
            obj = append_char(obj, *ptr);
            ptr++;
        }
    }
    va_end(va);
    return obj;
}
