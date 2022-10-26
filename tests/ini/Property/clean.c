#include <assert.h>

#include "my-db/ini/Property.h"

int main() {
    Property *property = createEmptyProperty("Prop");

    setNumber(property, 42);

    assert(property->type == PT_NUMBER);

    clean(property);

    assert(property->type == PT_NONE);

    freeProperty(property);
}