/*
    Copyright (c) 2020 Ivan Kniazkov <ivan.kniazkov.com>

    The implementation of the strings library
*/

#include "strings.h"
#include "allocator.h"
#include <stdio.h>

static const size_t initial_capacity = 16;

static __inline string_builder_t * instantiate_string_builder(size_t capacity)
{
    size_t size = sizeof(string_builder_t) + sizeof(char) * (capacity + 1);
    string_builder_t *obj = nnalloc(size);
    obj->data = (char*)(obj + 1);
    obj->capacity = capacity;
    return obj;
}

static __inline wide_string_builder_t * instantiate_wide_string_builder(size_t capacity)
{
    size_t size = sizeof(wide_string_builder_t) + sizeof(wchar_t) * (capacity + 1);
    wide_string_builder_t *obj = nnalloc(size);
    obj->data = (wchar_t*)(obj + 1);
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

wide_string_builder_t * create_wide_string_builder(size_t capacity)
{
    wide_string_builder_t *obj = instantiate_wide_string_builder(capacity);
    obj->data[0] = L'\0';
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

wide_string_builder_t * append_wide_char(wide_string_builder_t *obj, wchar_t ch)
{
    if (!obj)
    {
        obj = instantiate_wide_string_builder(initial_capacity);
        obj->data[0] = ch;
        obj->data[1] = L'\0';
        obj->length = 1;
        return obj;
    }
    else if (obj->capacity > obj->length)
    {
        obj->data[obj->length++] = ch;
        obj->data[obj->length] = L'\0';
        return obj;
    }
    else
    {
        size_t new_capacity = obj->capacity ? obj->capacity * 2 : initial_capacity;
        wide_string_builder_t *new_obj = instantiate_wide_string_builder(new_capacity);
        new_obj->length = obj->length + 1;
        memcpy(new_obj->data, obj->data, obj->length * sizeof(wchar_t));
        new_obj->data[obj->length] = ch;
        new_obj->data[new_obj->length] = L'\0';
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

wide_string_builder_t * append_non_wide_string(wide_string_builder_t *obj, string_t str)
{
    if (!obj)
    {
        size_t capacity = initial_capacity;
        if (capacity < str.length) capacity = str.length;
        obj = instantiate_wide_string_builder(capacity);
        wchar_t *dst = obj->data;
        char *src = str.data;
        size_t k = str.length;
        while(k--)
            *dst++ = *src++;
        *dst = L'\0';
        obj->length = str.length;
        return obj;
    }
    size_t new_length = obj->length + str.length;
    if (new_length <= obj->capacity)
    {
        wchar_t *dst = obj->data + obj->length;
        char *src = str.data;
        size_t k = str.length;
        while(k--)
            *dst++ = *src++;
        *dst = L'\0';
        obj->length = new_length;
        return obj;
    }
    else
    {
        size_t new_capacity = obj->capacity ? obj->capacity * 2 : initial_capacity;
        if (new_capacity < new_length) new_capacity = new_length;
        wide_string_builder_t *new_obj = instantiate_wide_string_builder(new_capacity);
        new_obj->length = new_length;
        memcpy(new_obj->data, obj->data, obj->length * sizeof(wchar_t));
        wchar_t *dst = new_obj->data + obj->length;
        char *src = str.data;
        size_t k = str.length;
        while(k--)
            *dst++ = *src++;
        *dst = L'\0';
        free(obj);
        return new_obj;
    }    
}

wide_string_builder_t * append_wide_string(wide_string_builder_t *obj, wide_string_t str)
{
    if (!obj)
    {
        size_t capacity = initial_capacity;
        if (capacity < str.length) capacity = str.length;
        obj = instantiate_wide_string_builder(capacity);
        memcpy(obj->data, str.data, str.length * sizeof(wchar_t));
        obj->data[str.length] = L'\0';
        obj->length = str.length;
        return obj;
    }
    size_t new_length = obj->length + str.length;
    if (new_length <= obj->capacity)
    {
        memcpy(obj->data + obj->length, str.data, str.length * sizeof(wchar_t));
        obj->data[new_length] = L'\0';
        obj->length = new_length;
        return obj;
    }
    else
    {
        size_t new_capacity = obj->capacity ? obj->capacity * 2 : initial_capacity;
        if (new_capacity < new_length) new_capacity = new_length;
        wide_string_builder_t *new_obj = instantiate_wide_string_builder(new_capacity);
        new_obj->length = new_length;
        memcpy(new_obj->data, obj->data, obj->length * sizeof(wchar_t));
        memcpy(new_obj->data + obj->length, str.data, str.length * sizeof(wchar_t));
        new_obj->data[new_obj->length] = L'\0';
        free(obj);
        return new_obj;
    }    
}

string_builder_t * append_formatted_string_ext(string_builder_t *obj, const char *format, va_list arg_list)
{
    const char *ptr = format;
    while(*ptr)
    {
        if (*ptr == '%')
        {
            ptr++;
            switch(*ptr)
            {
                case 'c':
                {
                    int ch = va_arg(arg_list, int);
                    obj = append_char(obj, (char)ch);
                    break;
                }

                case 's':
                {
                    const char *c_str = va_arg(arg_list, const char*);
                    obj = append_string(obj, _S(c_str));
                    break;
                }

                case 'S':
                {
                    string_t str = va_arg(arg_list, string_t);
                    obj = append_string(obj, str);
                    break;
                }

                case 'i':
                case 'd':
                {
                    int value = va_arg(arg_list, int);
                    char tmp[11];
                    sprintf(tmp, "%d", value);
                    obj = append_string(obj, _S(tmp));
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
    return obj;
}

wide_string_builder_t * append_formatted_wide_string_ext(wide_string_builder_t *obj, const wchar_t *format, va_list arg_list)
{
    const wchar_t *ptr = format;
    while(*ptr)
    {
        if (*ptr == '%')
        {
            ptr++;
            switch(*ptr)
            {
                case 'c':
                {
                    int ch = va_arg(arg_list, int);
                    obj = append_wide_char(obj, (wchar_t)ch);
                    break;
                }

                case 's':
                {
                    const char *c_str = va_arg(arg_list, const char*);
                    obj = append_non_wide_string(obj, _S(c_str));
                    break;
                }

                case 'S':
                {
                    string_t str = va_arg(arg_list, string_t);
                    obj = append_non_wide_string(obj, str);
                    break;
                }

                case 'w':
                {
                    const wchar_t *c_wstr = va_arg(arg_list, const wchar_t*);
                    obj = append_wide_string(obj, _W(c_wstr));
                    break;
                }

                case 'W':
                {
                    wide_string_t wstr = va_arg(arg_list, wide_string_t);
                    obj = append_wide_string(obj, wstr);
                    break;
                }

                case 'i':
                case 'd':
                {
                    int value = va_arg(arg_list, int);
                    char tmp[11];
                    sprintf(tmp, "%d", value);
                    obj = append_non_wide_string(obj, _S(tmp));
                    break;
                }

                default:
                    obj = append_wide_char(obj, *ptr);
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
            obj = append_wide_char(obj, *ptr);
            ptr++;
        }
    }
    return obj;
}

int compare_strings(string_t *first, string_t *second)
{
    return strcmp(first->data, second->data);
}

int compare_wide_strings(wide_string_t *first, wide_string_t *second)
{
    return wcscmp(first->data, second->data);
}

string_t * duplicate_string(string_t str)
{
    string_t *copy = nnalloc(sizeof(string_t) + (str.length + 1) * sizeof(char));
    copy->data = (char*)(copy + 1);
    copy->length = str.length;
    memcpy(copy->data, str.data, str.length * sizeof(char));
    copy->data[str.length] = '\0';
    return copy;
}

wide_string_t * duplicate_wide_string(wide_string_t str)
{
    wide_string_t *copy = nnalloc(sizeof(string_t) + (str.length + 1) * sizeof(wchar_t));
    copy->data = (wchar_t*)(copy + 1);
    copy->length = str.length;
    memcpy(copy->data, str.data, str.length * sizeof(wchar_t));
    copy->data[str.length] = L'\0';
    return copy;    
}
