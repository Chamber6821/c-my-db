#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    FILE *file = tmpfile();
    fputs("[empty section]\n", file);
    rewind(file);

    Config *config = readConfigFile(file);
    assert(vector_size(config->sections) == 0);

    freeConfig(config);
}