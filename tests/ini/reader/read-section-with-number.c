#include <assert.h>
#include <string.h>

#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    FILE *file = tmpfile();
    fputs("[section with number]\n", file);
    fputs("my favorite number=42.228", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *section = findSection(config, "section with number");
    assert(section != NULL);
    Property *property = findProperty(section, "my favorite number");
    assert(property != NULL);
    assert(property->type == PT_NUMBER);
    assert(getNumber(property) == 42.228f);

    freeConfig(config);
}