#include <assert.h>
#include <string.h>

#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    Config *config = readConfigString(
        "[my first section]\n"
        "ho-ho key = the best value");

    Section *section = findSection(config, "my first section");
    assert(section != NULL);
    Property *property = findProperty(section, "ho-ho key");
    assert(property != NULL);
    assert(property->type == PT_STRING);
    assert(strcmp(getString(property), "the best value") == 0);

    freeConfig(config);
}