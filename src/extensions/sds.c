#include "extensions/sds.h"

#include <string.h>

#include "c-vector/vec.h"

bool sdsstartwith(sds str, const char *prefix) {
    int i = 0;
    while (i < sdslen(str) && prefix[i]) {
        if (str[i] != prefix[i]) return false;
        i++;
    }
    return prefix[i] == 0;
}

vec_sds sdssplit(const char *s, const char *sep) {
    int count = -1;
    sds *tokens = sdssplitlen(s, strlen(s), sep, strlen(sep), &count);

    if (tokens == NULL) return NULL;
    if (count == 0) return NULL;

    vec_sds acc = vector_create();
    for (int i = 0; i < count; i++) {
        vector_add(&acc, sdsdup(tokens[i]));
    }
    sdsfreesplitres(tokens, count);

    return acc;
}
