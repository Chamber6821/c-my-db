#include <assert.h>
#include <c-vector/vec.h>

#include "my-db/ini/Section.h"

int main() {
    const char *name = "SectionWithoutPrefixes";
    Section *section = createEmptySection(name);

    vector_add(&section->properties, createEmptyProperty("Hm-hm prop"));
    vector_add(&section->properties, createEmptyProperty("Ho-ho prop"));
    vector_add(&section->properties, createEmptyProperty("He-he prop"));

    vec_Property properties =
        findPropertiesWithPrefix(section, "some not exists prefix");

    assert(properties == NULL);
}