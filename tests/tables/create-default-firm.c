#include <assert.h>
#include <string.h>

#include "my-db/tables/firm.h"

int main() {
    firm_t *firm = createDefaultFirm();

    assert(firm != NULL);
    assert(firm->id == -1);
    assert(firm->name != NULL);
    assert(strcmp(firm->name, "") == 0);
    assert(firm->workers != NULL);
    assert(strcmp(firm->workers, "") == 0);
    assert(firm->price == 0);
    assert(firm->monthTax == 0);

    freeFirm(firm);
}