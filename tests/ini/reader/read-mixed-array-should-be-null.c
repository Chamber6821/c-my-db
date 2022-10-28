#include <assert.h>
#include <string.h>

#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    FILE *file = tmpfile();
    fputs("[Trash]\n", file);
    fputs("mixed[]=asd\n", file);
    fputs("mixed[]=4.5\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    assert(config == NULL);
}
