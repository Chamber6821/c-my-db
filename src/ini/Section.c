#include "my-db/ini/Section.h"

#include <assert.h>
#include <c-vector/vec.h>
#include <malloc.h>
#include <string.h>

Section* createEmptySection(const char* name) {
    Section* new = malloc(sizeof(Section));
    assert(new != NULL);

    new->name = strdup(name);
    assert(new->name != NULL);

    new->properties = vector_create();
    assert(new->properties != NULL);

    return new;
}

void freeSection(Section* this) {
    for (int i = 0; i < vector_size(this->properties); i++) {
        freeProperty(this->properties[i]);
    }
    vector_free(this->properties);
    free(this->name);
    free(this);
}

Property* findProperty(Section* this, const char* propertyName) {
    for (int i = 0; i < vector_size(this->properties); i++) {
        if (strcmp(this->properties[i]->name, propertyName) == 0) {
            return this->properties[i];
        }
    }
    return NULL;
}