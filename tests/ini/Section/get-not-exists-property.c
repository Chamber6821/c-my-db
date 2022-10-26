#include <assert.h>
#include <c-vector/vec.h>

#include "my-db/ini/Section.h"

int main() {
    const char *name = "TestSection";
    Section *section = createEmptySection(name);

    Property *property = createEmptyProperty("MyProp");
    vector_add(&section->properties, property);

    Property *gotProperty = findProperty(section, "MyAnotherProp");

    assert(gotProperty == NULL);

    freeSection(section);
}