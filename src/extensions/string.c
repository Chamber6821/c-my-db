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

bool strendwith(const char *str, const char *suffix) {
    size_t strLen = strlen(str);
    size_t suffixLen = strlen(suffix);

    if (suffixLen == 0) return true;
    if (suffixLen > strLen) return false;

    size_t offset = strLen - suffixLen;
    return strcmp(str + offset, suffix) == 0;
}
