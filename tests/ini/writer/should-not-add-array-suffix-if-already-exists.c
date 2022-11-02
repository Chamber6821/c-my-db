#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "extensions/vec.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/writer.h"

bool fileEqualString(FILE *file, const char *str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        char ch = fgetc(file);
        if (ch == EOF) return false;
        if (ch != str[i]) return false;
    }

    return fgetc(file) == EOF;
}

int main() {
    Config *config = createEmptyConfig();

    vec_sds names = vector_create();
    vector_add(&names, sdsnew("Mike"));

    copyToStrings(addProperty(addSection(config, "Sec"), "names[]"), names);
    vector_deep_free(names, sdsfree, i);

    char expected[] =
        "[Sec]\n"
        "names[]=\"Mike\"\n";

    FILE *file = tmpfile();
    dumpConfigToFile(file, config);
    rewind(file);

    assert(fileEqualString(file, expected));

    freeConfig(config);
}