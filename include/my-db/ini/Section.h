#pragma once

#include "Property.h"

typedef Property **vec_Property;

typedef struct {
    char *name;
    vec_Property properties;
} Section;

Section *createEmptySection(const char *name);
void freeSection(Section *this);

Property *findProperty(Section *this, const char *propertyName);