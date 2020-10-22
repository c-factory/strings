/*
    Copyright (c) 2020 Ivan Kniazkov <ivan.kniazkov.com>

    The declaration of the 'strings' library:
    structs representing extended strings
*/

#pragma once

#include <string.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdbool.h>

typedef struct
{
    char * data;
    size_t length;
} string_t;

static __inline string_t init_string(const char * data, size_t length)
{
    string_t s = { (char*)data, length };
    return s;
}

#define _S(s) init_string(s, strlen(s))
#define __S(s) init_string(s, (sizeof(s) - 1) / sizeof(char))

typedef struct
{
    wchar_t * data;
    size_t length;
} wide_string_t;

static __inline wide_string_t init_wide_string(const wchar_t * data, size_t length)
{
    wide_string_t ws = { (wchar_t*)data, length };
    return ws;
}

#define _W(ws) init_wide_string(ws, wcslen(ws))
#define __W(ws) init_wide_string(ws, (sizeof(ws) - 1) / sizeof(wchar_t))

typedef struct
{
    char * data;
    size_t length;
    size_t capacity;
} string_builder_t;

typedef struct
{
    wchar_t * data;
    size_t length;
    size_t capacity;
} wide_string_builder_t;

string_builder_t * create_string_builder(size_t capacity);
wide_string_builder_t * create_wide_string_builder(size_t capacity);
string_builder_t * append_char(string_builder_t *obj, char ch);
wide_string_builder_t * append_wide_char(wide_string_builder_t *obj, wchar_t ch);
string_builder_t * append_string(string_builder_t *obj, string_t str);
wide_string_builder_t * append_non_wide_string(wide_string_builder_t *obj, string_t str);
wide_string_builder_t * append_wide_string(wide_string_builder_t *obj, wide_string_t str);
string_builder_t * append_formatted_string_ext(string_builder_t *obj, const char *format, va_list arg_list);
wide_string_builder_t * append_formatted_wide_string_ext(wide_string_builder_t *obj, const wchar_t *format, va_list arg_list);
int compare_strings(string_t *first, string_t *second);
int compare_wide_strings(wide_string_t *first, wide_string_t *second);
string_t * duplicate_string(string_t str);
wide_string_t * duplicate_wide_string(wide_string_t str);

static __inline string_t * create_formatted_string(const char *format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);
    string_builder_t *result = append_formatted_string_ext(NULL, format, arg_list);
    va_end(arg_list);
    return (string_t*)result;
}

static __inline wide_string_t * create_formatted_wide_string(const wchar_t *format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);
    wide_string_builder_t *result = append_formatted_wide_string_ext(NULL, format, arg_list);
    va_end(arg_list);
    return (wide_string_t*)result;
}

static __inline string_builder_t * append_formatted_string(string_builder_t *obj, const char *format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);
    string_builder_t *result = append_formatted_string_ext(obj, format, arg_list);
    va_end(arg_list);
    return result;
}

static __inline wide_string_builder_t * append_formatted_wide_string(wide_string_builder_t *obj, const wchar_t *format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);
    wide_string_builder_t *result = append_formatted_wide_string_ext(obj, format, arg_list);
    va_end(arg_list);
    return result;
}

static __inline bool are_strings_equal(string_t first, string_t second)
{
    return first.length == second.length && 0 == strcmp(first.data, second.data);
}

static __inline bool are_wide_strings_equal(wide_string_t first, wide_string_t second)
{
    return first.length == second.length && 0 == wcscmp(first.data, second.data);
}

wide_string_t * decode_utf8_string(string_t str);
string_t * encode_utf8_string(wide_string_t wstr);
string_t * read_file_to_string(const char *file_name);
