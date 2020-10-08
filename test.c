#include <stdio.h>
#include "strings.h"

int main(void)
{
    string_builder_t *b = create_string_builder(0);
    b = append_char(b, 'A');
    b = append_char(b, 'B');
    b = append_char(b, 'C');
    b = append_char(b, 'D');
    b = append_char(b, 'E');
    b = append_char(b, 'F');
    b = append_char(b, 'G');
    b = append_char(b, 'H');
    b = append_char(b, 'I');
    b = append_char(b, 'J');
    b = append_char(b, 'K');
    b = append_char(b, 'L');
    b = append_char(b, 'M');
    b = append_char(b, 'N');
    b = append_char(b, 'O');
    b = append_char(b, 'P');
    b = append_char(b, 'Q');
    string_t *s = (string_t*)b;
    printf("%s\n", s->data);
    return 0;
}
