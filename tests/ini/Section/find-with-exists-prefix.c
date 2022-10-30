#include <assert.h>
#include <c-vector/vec.h>

#include "my-db/ini/Section.h"

int main() {
    const char *name = "SectionWithoutPrefixes";
    Section *section = createEmptySection(name);

    Property *propA = createEmptyProperty("field_A");
    Property *propB = createEmptyProperty("field_B");

    vector_add(&section->properties, createEmptyProperty("some trash"));
    vector_add(&section->properties, propA);
    vector_add(&section->properties, propB);
    vector_add(&section->properties, createEmptyProperty("he-he"));

    vec_Property properties = findPropertiesWithPrefix(section, "field_");

    assert(properties != NULL);
    assert(vector_size(properties) == 2);
    assert(properties[0] == propA);
    assert(properties[1] == propB);

    freeSection(section);
    vector_free(properties);  // Property* was free in freeSection
}