#include <stdio.h>
#include <stdlib.h>
#include "strings.h"

int main(void)
{
    string_t *s = create_formatted_string("%s the new look %c to the %S %d", "hello, it is", '#', _S("pure C"), 1024);
    printf("%d\n", s->length);
    printf("%s\n", s->data);
    free(s);
    return 0;
}
