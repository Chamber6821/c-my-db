#include <assert.h>
#include <string.h>

#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    FILE *file = tmpfile();
    fputs("[Quotes]\n", file);
    fputs("number with double quotes=\"42\"\n", file);
    fputs("number with single quotes='228'\n", file);
    fputs("string with double quotes=\"Sasha\"\n", file);
    fputs("string with single quotes='Dog'\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *section = findSection(config, "Quotes");
    assert(section != NULL);

    Property *property = findProperty(section, "number with double quotes");
    assert(property != NULL);
    assert(property->type == PT_STRING);
    assert(strcmp(getString(property), "42") == 0);

    property = findProperty(section, "number with single quotes");
    assert(property != NULL);
    assert(property->type == PT_STRING);
    assert(strcmp(getString(property), "228") == 0);

    property = findProperty(section, "string with double quotes");
    assert(property != NULL);
    assert(property->type == PT_STRING);
    assert(strcmp(getString(property), "Sasha") == 0);

    property = findProperty(section, "string with single quotes");
    assert(property != NULL);
    assert(property->type == PT_STRING);
    assert(strcmp(getString(property), "Dog") == 0);

    freeConfig(config);
}