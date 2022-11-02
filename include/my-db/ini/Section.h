#pragma once

#include "Property.h"

typedef Property **vec_Property;

typedef struct {
    sds name;
    vec_Property properties;
} Section;

Section *createEmptySection(const char *name);
void freeSection(Section *this);

Property *addProperty(Section *this, const char *propName);

Property *findProperty(Section *this, const char *propertyName);
vec_Property findPropertiesWithPrefix(Section *this, const char *prefix);
