#include <assert.h>
#include <stdio.h>
#include <string.h>

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
    copyToString(addProperty(addSection(config, "Sec"), "string"), "Hi, Mike");

    char expected[] =
        "[Sec]\n"
        "string=\"Hi, Mike\"\n";

    FILE *file = tmpfile();
    dumpConfigToFile(file, config);
    rewind(file);

    assert(fileEqualString(file, expected));

    freeConfig(config);
}