#include <assert.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    FILE *file = tmpfile();
    fputs("[World]\n", file);
    fputs("cities[]=Minsk\n", file);
    fputs("cities[]=Moscow\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *section = findSection(config, "World");
    assert(section != NULL);
    Property *property = findProperty(section, "cities[]");
    assert(property != NULL);
    assert(property->type == PT_ARRAY_STRING);

    vec_sds cities = getStrings(property);
    assert(cities != NULL);
    assert(vector_size(cities) == 2);
    assert(strcmp(cities[0], "Minsk") == 0);
    assert(strcmp(cities[1], "Moscow") == 0);

    freeConfig(config);
}
