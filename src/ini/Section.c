#include "my-db/ini/Section.h"

#include <assert.h>
#include <c-vector/vec.h>
#include <malloc.h>
#include <string.h>

#include "extensions/sds.h"

Section *createEmptySection(const char *name) {
    Section *new = malloc(sizeof(Section));
    assert(new != NULL);

    new->name = sdsnew(name);
    assert(new->name != NULL);

    new->properties = vector_create();
    assert(new->properties != NULL);

    return new;
}

void freeSection(Section *this) {
    if (this == NULL) return;
    for (int i = 0; i < vector_size(this->properties); i++) {
        freeProperty(this->properties[i]);
    }
    vector_free(this->properties);
    sdsfree(this->name);
    free(this);
}

Property *findProperty(Section *this, const char *propertyName) {
    for (int i = 0; i < vector_size(this->properties); i++) {
        if (strcmp(this->properties[i]->name, propertyName) == 0) {
            return this->properties[i];
        }
    }
    return NULL;
}
vec_Property findPropertiesWithPrefix(Section *this, const char *prefix) {
    vec_Property acc = vector_create();
    for (int i = 0; i < vector_size(this->properties); i++) {
        if (sdsstartwith(this->properties[i]->name, prefix)) {
            vector_add(&acc, this->properties[i]);
        }
    }

    if (vector_size(acc) == 0) {
        vector_free(acc);
        return NULL;
    }

    return acc;
}
