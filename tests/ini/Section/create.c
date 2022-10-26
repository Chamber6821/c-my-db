#include <assert.h>
#include <c-vector/vec.h>
#include <stddef.h>
#include <string.h>

#include "my-db/ini/Section.h"

int main() {
    const char *name = "TestSection";
    Section *section = createEmptySection(name);

    assert(section != NULL);

    assert(section->properties != NULL);
    assert(vector_size(section->properties) == 0);

    assert(section->name != name);
    assert(strcmp(section->name, name) == 0);

    freeSection(section);
}