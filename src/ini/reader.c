#include "my-db/ini/reader.h"

#include <string.h>

#include "c-vector/vec.h"
#include "inih/ini.h"
#include "sds/sds.h"

const char* ARRAY_SUFFIX = "[]";

static bool isArrayElement(const char* key) {
    sds copy = sdsnew(key);
    sdsrange(copy, -2, -1);
    bool result = strcmp(copy, ARRAY_SUFFIX) == 0;
    sdsfree(copy);
    return result;
}

static bool tryParseNumber(const char* value, float* out) {
    char* end;
    *out = strtof(value, &end);
    return (end - value) == strlen(value);
}

static bool tryAddNumber(Property* prop, float number) {
    if (!hasValue(prop)) {
        vec_float empty = vector_create();
        copyToNumbers(prop, empty);
        vector_free(empty);
    } else if (getNumbers(prop) == NULL) {
        return false;
    }

    vector_add(&prop->numbers, number);
    return true;
}

static bool tryAddString(Property* prop, const char* str) {
    if (!hasValue(prop)) {
        vec_sds empty = vector_create();
        copyToStrings(prop, empty);
        vector_free(empty);
    } else if (getStrings(prop) == NULL) {
        return false;
    }

    vector_add(&prop->strings, sdsnew(str));
    return true;
}

static int handler(void* user, const char* sectionName, const char* key,
                   const char* value) {
    Config* config = (Config*)user;

    Section* section = findSection(config, sectionName);
    if (section == NULL) {
        section = createEmptySection(sectionName);
        vector_add(&config->sections, section);
    }

    Property* property = findProperty(section, key);
    if (property == NULL) {
        property = createEmptyProperty(key);
        vector_add(&section->properties, property);
    }

    if (isArrayElement(key)) {
        float number;
        if (tryParseNumber(value, &number)) {
            if (!tryAddNumber(property, number)) {
                return 0;  // type error
            }
            return 1;
        }

        if (!tryAddString(property, value)) {
            return 0;  // type error
        }
        return 1;
    }

    float number;
    if (tryParseNumber(value, &number)) {
        setNumber(property, number);
        return 1;
    }

    copyToString(property, value);
    return 1;
}

Config* readConfigFile(FILE* source) {
    Config* config = createEmptyConfig();
    int code = ini_parse_file(source, handler, config);

    if (code != 0) {
        freeConfig(config);
        fprintf(stderr, "Parsing INI file error: %d", code);
        return NULL;
    }

    return config;
}
