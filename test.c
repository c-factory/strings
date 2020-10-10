#include <stdio.h>
#include <stdlib.h>
#include "strings.h"

int main(void)
{
    wide_string_t *s = create_formatted_wide_string(L"a + b = %d", 1024);
    wprintf(L"%s\n", s->data);
    free(s);
    return 0;
}
