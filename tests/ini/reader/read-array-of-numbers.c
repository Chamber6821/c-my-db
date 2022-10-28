#include <assert.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    FILE *file = tmpfile();
    fputs("[Uni]\n", file);
    fputs("marks[]=3\n", file);
    fputs("marks[]=4.5\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *section = findSection(config, "Uni");
    assert(section != NULL);
    Property *property = findProperty(section, "marks[]");
    assert(property != NULL);
    assert(property->type == PT_ARRAY_NUMBER);

    vec_float marks = getNumbers(property);
    assert(marks != NULL);
    assert(vector_size(marks) == 2);
    assert(marks[0] == 3);
    assert(marks[1] = 4.5);

    freeConfig(config);
}
