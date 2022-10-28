#include "extensions/string.h"

#include <string.h>

int strstrcount(char *str, const char *substr) {
    int substrLen = strlen(substr);
    if (substrLen == 0) return 0;

    int count = 0;
    const char *p = strstr(str, substr);
    while (p != NULL) {
        count++;
        p = strstr(p + substrLen, substr);
    }
    return count;
}
