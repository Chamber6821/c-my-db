#include <assert.h>
#include <c-vector/vec.h>

#include "my-db/ini/Property.h"

int main() {
    Property *property = createEmptyProperty("Prop");

    vec_float numbers = vector_create();
    vector_add(&numbers, 42.228f);
    vector_add(&numbers, 96);
    copyToNumbers(property, numbers);

    vector_pop(numbers);
    numbers[0] = -1;

    vec_float gotNumbers = getNumbers(property);

    assert(vector_size(gotNumbers) == 2);
    assert(gotNumbers[0] == 42.228f);
    assert(gotNumbers[1] == 96);

    vector_free(numbers);
    freeProperty(property);
}