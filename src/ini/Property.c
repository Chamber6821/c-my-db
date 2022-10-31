#include "my-db/ini/Property.h"

#include <assert.h>
#include <c-vector/vec.h>
#include <malloc.h>

Property *createEmptyProperty(const char *name) {
    Property *new = malloc(sizeof(Property));
    assert(new != NULL);

    new->type = PT_NONE;
    new->name = sdsnew(name);
    assert(new->name != NULL);

    return new;
}

void freeProperty(Property *this) {
    if (this == NULL) return;
    clean(this);
    sdsfree(this->name);
    free(this);
}

bool hasValue(Property *this) { return this->type != PT_NONE; }

void clean(Property *this) {
    switch (this->type) {
        case PT_STRING: {
            sdsfree(this->string);
            break;
        }
        case PT_ARRAY_STRING: {
            for (int i = 0; i < vector_size(this->strings); i++) {
                sdsfree(this->strings[i]);
            }
            vector_free(this->strings);
            break;
        }
        case PT_ARRAY_NUMBER: {
            vector_free(this->numbers);
            break;
        }
        case PT_NONE:
        case PT_NUMBER:
            break;
    }

    this->type = PT_NONE;
}

float getNumber(Property *this) {
    if (this->type != PT_NUMBER) return 0;
    return this->number;
}

void setNumber(Property *this, float value) {
    clean(this);

    this->type = PT_NUMBER;
    this->number = value;
}

vec_float getNumbers(Property *this) {
    if (this->type != PT_ARRAY_NUMBER) return NULL;
    return this->numbers;
}

void copyToNumbers(Property *this, vec_float what) {
    clean(this);

    if (what == NULL) return;

    this->type = PT_ARRAY_NUMBER;
    this->numbers = vector_copy(what);
}

sds getString(Property *this) {
    if (this->type != PT_STRING) return NULL;
    return this->string;
}

void copyToString(Property *this, const char *what) {
    clean(this);

    if (what == NULL) return;

    this->type = PT_STRING;
    this->string = sdsnew(what);
}

vec_sds getStrings(Property *this) {
    if (this->type != PT_ARRAY_STRING) return NULL;
    return this->strings;
}

void copyToStrings(Property *this, vec_sds what) {
    clean(this);

    if (what == NULL) return;

    this->type = PT_ARRAY_STRING;
    this->strings = vector_create();
    for (int i = 0; i < vector_size(what); i++) {
        vector_add(&this->strings, sdsdup(what[i]));
    }
}
