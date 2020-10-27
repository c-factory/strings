/*
    Copyright (c) 2020 Ivan Kniazkov <ivan.kniazkov.com>

    The implementation of the strings library
*/

#include "strings.h"
#include "allocator.h"
#include <stdio.h>
#include <stdint.h>

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

                case 'f':
                {
                    double value = va_arg(arg_list, double);
                    char tmp[11];
                    sprintf(tmp, "%f", value);
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

                case 'f':
                {
                    double value = va_arg(arg_list, double);
                    char tmp[11];
                    sprintf(tmp, "%f", value);
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

wide_string_t *decode_utf8_string(string_t str)
{
    size_t i = 0;
    wide_string_builder_t *b = NULL;
    while (i < str.length)
    {
        unsigned char c0 = (unsigned char)str.data[i];
        wchar_t w = 0;
        if ((c0 & 0x80) == 0)
        {
            w = c0;
            i++;
        }
        else if ((c0 & 0xE0) == 0xC0)
        {
            if (i + 1 >= str.length)
                goto error;
            unsigned char c1 = (unsigned char)str.data[i + 1];
            if ((c1 & 0xC0) != 0x80)
                goto error;
            w = ((c0 & 0x1F) << 6) + (c1 & 0x3F);
            i += 2;
        }
        else if ((c0 & 0xF0) == 0xE0)
        {
            if (i + 2 >= str.length)
                goto error;
            unsigned char c1 = (unsigned char)str.data[i + 1];
            unsigned char c2 = (unsigned char)str.data[i + 2];
            if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80)
                goto error;
            w = ((c0 & 0xF) << 12) + ((c1 & 0x3F) << 6) + (c2 & 0x3F);
            i += 3;
        }
#if WCHAR_MAX > 0xFFFF
        else if ((c0 & 0xF8) == 0xF0)
        {
            if (i + 3 >= str.data)
                goto error;
            unsigned char c1 = (unsigned char)str.data[i + 1];
            unsigned char c2 = (unsigned char)str.data[i + 2];
            unsigned char c3 = (unsigned char)str.data[i + 3];
            if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80)
                goto error;
            w = ((c0 & 0x7) << 18) + ((c1 & 0x3F) << 12) + ((c2 & 0x3F) << 6) + (c3 & 0x3F);
            i += 4;
        }
#endif
        else
        {
            goto error;
        }
        b = append_wide_char(b, w);
    }
    if (!b)
        b = create_wide_string_builder(0);
    return (wide_string_t*)b;

error:
    free(b);
    return NULL;
}

static int encode_utf8_char(wchar_t w, char *c)
{
    if (w < 0x80)
    {
        c[0] = (char)w;
        return 1;
    }
    if (w < 0x800)
    {
        c[0] = (char)((w & 0x7C0) >> 6) + 0xC0;
        c[1] = (char)(w & 0x3F) + 0x80;
        return 2;
    }
    if (w < 0x10000)
    {
        c[0] = (char)((w & 0xF000) >> 12) + 0xE0;
        c[1] = (char)((w & 0xFC0) >> 6) + 0x80;
        c[2] = (char)(w & 0x3F) + 0x80;
        return 3;
    }
#if WCHAR_MAX > 0xFFFF
    if (w < 0x200000)
    {
        c[0] = (char)((w & 0x1C0000) >> 18) + 0xF0;
        c[1] = (char)((w & 0x3F000) >> 12) + 0x80;
        c[2] = (char)((w & 0xFC0) >> 6) + 0x80;
        c[3] = (char)(w & 0x3F) + 0x80;
        return 3;
    }
#endif
    return 0;
}

string_t * encode_utf8_string(wide_string_t wstr)
{
    string_builder_t *b = NULL;
    int i, n;
    char tmp[4];
    for (size_t k = 0; k < wstr.length; k++)
    {
        wchar_t w = wstr.data[k];
        n = encode_utf8_char(w, tmp);
        if (n > 0)
        {
            for (i = 0; i < n; i++)
                b = append_char(b, tmp[i]);
        }
        else
            b = append_char(b, '?');
    }
    if (!b)
        b = create_string_builder(0);
    return (string_t*)b;
}

string_t * read_file_to_string(const char *file_name)
{
    FILE *stream = fopen(file_name, "rb");
    if (!stream)
        return NULL;
    fseek(stream, 0, SEEK_END);
    long file_size = ftell(stream);
    const size_t max_size = SIZE_MAX - (sizeof(string_t) - 1);
    if (file_size > max_size) file_size = max_size;
    size_t length = (size_t)file_size;
    string_t *str = nnalloc(sizeof(string_t) + length + 1);
    str->data = (char*)(str + 1);
    str->length = length;
    str->data[length] = 0;
    rewind(stream);
    size_t count = fread(str->data, 1, length, stream);
    fclose(stream);
    if (count < length)
    {
        free(str);
        return  NULL;
    }
    return str;
}

string_t * wide_string_to_string(wide_string_t wstr, char bad_char, bool *was_bad_char)
{
    bool bad_flag = false;
    string_t * str = nnalloc(sizeof(string_t) + (wstr.length + 1) * sizeof(char));
    str->data = (char*)(str + 1);
    str->data[wstr.length] = '\0';
    str->length = wstr.length;
    for (size_t i = 0; i < wstr.length; i++)
    {
        wchar_t c = wstr.data[i];
        if (c < 128)
        {
            str->data[i] = (char)c;    
        }
        else
        {
            str->data[i] = bad_char;
            bad_flag = true;
        }
    }
    if (was_bad_char) *was_bad_char = bad_flag;
    return str;
}

string_t * sub_string(string_t str, size_t index, size_t length)
{
    if (index > str.length) index = str.length;
    size_t tail = str.length - index;
    if (length > tail) length = tail;
    string_t *result = nnalloc(sizeof(string_t) + (length + 1) * sizeof(char));
    result->data = (char*)(result + 1);
    result->data[length] = '\0';
    memcpy(result->data, str.data + index, length * sizeof(char));
    result->length = length;
    return result;
}

wide_string_t * sub_wide_string(wide_string_t wstr, size_t index, size_t length)
{
    if (index > wstr.length) index = wstr.length;
    size_t tail = wstr.length - index;
    if (length > tail) length = tail;
    wide_string_t *result = nnalloc(sizeof(wide_string_t) + (length + 1) * sizeof(wchar_t));
    result->data = (wchar_t*)(result + 1);
    result->data[length] = L'\0';
    memcpy(result->data, wstr.data + index, length * sizeof(wchar_t));
    result->length = length;
    return result;
}

size_t index_of_char_in_string(string_t str, char ch)
{
    size_t i = 0;
    for (; i < str.length; i++)
    {
        if (str.data[i] == ch)
            break;
    }
    return i;
}

size_t index_of_char_in_wide_string(wide_string_t wstr, wchar_t ch)
{
    size_t i = 0;
    for (; i < wstr.length; i++)
    {
        if (wstr.data[i] == ch)
            break;
    }
    return i;
}
