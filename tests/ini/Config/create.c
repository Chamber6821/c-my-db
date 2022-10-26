#include <assert.h>
#include <c-vector/vec.h>
#include <stddef.h>

#include "my-db/ini/Config.h"

int main() {
    Config *config = createEmptyConfig();

    assert(config != NULL);

    assert(config->sections != NULL);
    assert(vector_size(config->sections) == 0);

    freeConfig(config);
}