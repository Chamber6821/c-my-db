#include <assert.h>
#include <c-vector/vec.h>

#include "my-db/ini/Config.h"

int main() {
    Config *config = createEmptyConfig();

    Section *section = createEmptySection("MySection");
    vector_add(&config->sections, section);

    Section *gotSection = findSection(config, "MyAnotherSection");

    assert(gotSection == NULL);

    freeConfig(config);
}