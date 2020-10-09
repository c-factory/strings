/*
    Copyright (c) 2020 Ivan Kniazkov <ivan.kniazkov.com>

    The declaration of the 'strings' library:
    structs representing extended strings
*/

#pragma once

#include <string.h>
#include <wchar.h>

typedef struct
{
    char * data;
    size_t length;
} string_t;

typedef struct
{
    char * data;
    size_t length;
    size_t capacity;
} string_builder_t;


string_builder_t * create_string_builder(size_t capacity);
string_builder_t * append_char(string_builder_t *obj, char ch);
string_builder_t * append_string(string_builder_t *obj, string_t str);
string_builder_t * append_formatted_string(string_builder_t *obj, const char *format, ...);

static __inline string_t _S(const char *c_str)
{
    string_t result = { (char*)c_str, strlen(c_str) };
    return result;
}
