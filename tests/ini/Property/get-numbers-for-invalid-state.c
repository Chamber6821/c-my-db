#include <assert.h>
#include <stddef.h>

#include "my-db/ini/Property.h"

int main() {
    Property *property = createEmptyProperty("TestProperty");

    vec_float gotNumbers = getNumbers(property);

    assert(property->type == PT_NONE);
    assert(gotNumbers == NULL);

    freeProperty(property);
}