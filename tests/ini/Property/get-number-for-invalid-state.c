#include <assert.h>

#include "my-db/ini/Property.h"

int main() {
    Property *property = createEmptyProperty("Property");

    assert(0 == getNumber(property));

    freeProperty(property);
}