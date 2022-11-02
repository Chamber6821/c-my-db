#include "my-db/ini/writer.h"

#include <string.h>

#include "c-vector/vec.h"
#include "extensions/string.h"

bool dumpConfigToFile(FILE *file, Config *config) {
    if (!file || !config) return false;

    for (int i = 0; i < vector_size(config->sections); i++) {
        if (!dumpSectionToFile(file, config->sections[i])) return false;
    }

    return true;
}

static bool dumpProperty(FILE *file, Property *prop) {
    if (!prop) return false;

    switch (prop->type) {
        case PT_NONE:
            break;  // nothing
        case PT_STRING: {
            fprintf(file, "%s=\"%s\"\n", prop->name, getString(prop));
            break;
        }
        case PT_NUMBER: {
            fprintf(file, "%s=%g\n", prop->name, getNumber(prop));
            break;
        }
        case PT_ARRAY_STRING: {
            sds name = sdsdup(prop->name);
            if (!strendwith(name, "[]")) name = sdscat(name, "[]");

            vec_sds strings = getStrings(prop);
            for (int i = 0; i < vector_size(strings); i++) {
                fprintf(file, "%s=\"%s\"\n", name, strings[i]);
            }

            break;
        }
        case PT_ARRAY_NUMBER: {
            sds name = sdsdup(prop->name);
            if (!strendwith(name, "[]")) name = sdscat(name, "[]");

            vec_float numbers = getNumbers(prop);
            for (int i = 0; i < vector_size(numbers); i++) {
                fprintf(file, "%s=%g\n", name, numbers[i]);
            }

            break;
        }
    }

    return true;
}

bool dumpSectionToFile(FILE *file, Section *section) {
    if (!file || !section) return false;

    fprintf(file, "[%s]\n", section->name);

    for (int i = 0; i < vector_size(section->properties); i++) {
        if (!dumpProperty(file, section->properties[i])) return false;
    }

    return true;
}
