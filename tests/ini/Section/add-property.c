#include <assert.h>
#include <stddef.h>

#include "c-vector/vec.h"
#include "my-db/ini/Section.h"

int main() {
    Section *section = createEmptySection("MySection");
    assert(section != NULL);

    Property *prop = addEmptyProperty(section, "NewProp");
    assert(prop != NULL);
    assert(prop->type == PT_NONE);

    assert(vector_size(section->properties) == 1);

    freeSection(section);
}