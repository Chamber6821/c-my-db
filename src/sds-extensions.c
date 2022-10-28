#include "my-db/sds-extensions.h"

bool sdsstartwith(sds str, const char *prefix) {
    int i = 0;
    while (i < sdslen(str) && prefix[i]) {
        if (str[i] != prefix[i]) return false;
        i++;
    }
    return prefix[i] == 0;
}
