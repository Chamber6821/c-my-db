#include "my-db/ini/Config.h"

#include <assert.h>
#include <c-vector/vec.h>
#include <malloc.h>
#include <string.h>

Config *createEmptyConfig() {
    Config *new = malloc(sizeof(Config));
    assert(new != NULL);

    new->sections = vector_create();
    assert(new->sections != NULL);

    return new;
}

void freeConfig(Config *this) {
    if (this == NULL) return;
    for (int i = 0; i < vector_size(this->sections); i++) {
        freeSection(this->sections[i]);
    }
    vector_free(this->sections);
    free(this);
}

Section *addSection(Config *this, const char *sectionName) {
    Section *section = createEmptySection(sectionName);
    vector_add(&this->sections, section);
    return section;
}

Section *findSection(Config *this, const char *sectionName) {
    for (int i = 0; i < vector_size(this->sections); i++) {
        if (strcmp(this->sections[i]->name, sectionName) == 0) {
            return this->sections[i];
        }
    }
    return NULL;
}
