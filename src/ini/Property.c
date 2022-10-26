#include "my-db/ini/Property.h"

#include <assert.h>
#include <c-vector/vec.h>
#include <malloc.h>
#include <string.h>

Property* createEmptyProperty(const char* name) {
    Property* new = malloc(sizeof(Property));
    assert(new != NULL);

    new->type = PT_NONE;
    new->name = strdup(name);
    assert(new->name != NULL);

    return new;
}

void freeProperty(Property* this) {
    clean(this);
    free(this->name);
    free(this);
}

void clean(Property* this) {
    switch (this->type) {
        case PT_STRING: {
            free(this->string);
            break;
        }
        case PT_ARRAY_STRING: {
            for (int i = 0; i < vector_size(this->strings); i++) {
                free(this->strings[i]);
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

float getNumber(Property* this) {
    if (this->type != PT_NUMBER) return 0;
    return this->number;
}

void setNumber(Property* this, float value) {
    clean(this);

    this->type = PT_NUMBER;
    this->number = value;
}

vec_float getNumbers(Property* this) {
    if (this->type != PT_ARRAY_NUMBER) return NULL;
    return this->numbers;
}

void copyToNumbers(Property* this, vec_float what) {
    clean(this);

    if (what == NULL) return;

    this->type = PT_ARRAY_NUMBER;
    this->numbers = vector_copy(what);
}

char* getString(Property* this) {
    if (this->type != PT_STRING) return NULL;
    return this->string;
}

void copyToString(Property* this, const char* what) {
    clean(this);

    if (what == NULL) return;

    this->type = PT_STRING;
    this->string = strdup(what);
}

vec_string getStrings(Property* this) {
    if (this->type != PT_ARRAY_STRING) return NULL;
    return this->strings;
}

void copyToStrings(Property* this, vec_string what) {
    clean(this);

    if (what == NULL) return;

    this->type = PT_ARRAY_STRING;
    this->strings = vector_copy(what);
}
