#include <stdio.h>
#include "strings.h"

int main(void)
{
    string_builder_t *b = NULL;
    b = append_formatted_string(b, "%s the new look to the %s language", "hello, it is", "pure C");
    string_t *s = (string_t*)b;
    printf("%s\n", s->data);
    return 0;
}
