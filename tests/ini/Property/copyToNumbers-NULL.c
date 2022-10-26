#include <assert.h>
#include <stddef.h>

#include "my-db/ini/Property.h"

int main() {
    Property *property = createEmptyProperty("Prop");

    copyToNumbers(property, NULL);

    assert(property->type == PT_NONE);
    assert(getNumbers(property) == NULL);

    freeProperty(property);
}