#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "my-db/ini/Property.h"

int main() {
    Property *property = createEmptyProperty("TestProperty");

    assert(property != NULL);
    assert(property->type == PT_NONE);
    assert(property->name != NULL);
    assert(strcmp(property->name, "TestProperty") == 0);

    freeProperty(property);
}