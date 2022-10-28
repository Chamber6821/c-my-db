#pragma once

#include <stdbool.h>

#include "sds/sds.h"

typedef float *vec_float;
typedef sds *vec_sds;

typedef enum {
    PT_NONE,
    PT_STRING,
    PT_NUMBER,
    PT_ARRAY_STRING,
    PT_ARRAY_NUMBER,
} PropertyType;

typedef struct {
    sds name;
    PropertyType type;
    union {
        float number;
        sds string;

        vec_float numbers;
        vec_sds strings;
    };
} Property;

Property *createEmptyProperty(const char *name);
void freeProperty(Property *this);

bool hasValue(Property *this);

void clean(Property *this);

float getNumber(Property *this);
void setNumber(Property *this, float value);

vec_float getNumbers(Property *this);
void copyToNumbers(Property *this, vec_float what);

sds getString(Property *this);
void copyToString(Property *this, const char *what);

vec_sds getStrings(Property *this);
void copyToStrings(Property *this, vec_sds what);
