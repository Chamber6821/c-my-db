#include <assert.h>

#include "my-db/ini/Property.h"

int main() {
    Property *property = createEmptyProperty("TestProperty");

    setNumber(property, 42.228f);

    assert(property->type == PT_NUMBER);
    assert(getNumber(property) == 42.228f);

    freeProperty(property);
}