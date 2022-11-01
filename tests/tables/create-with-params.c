#include <assert.h>
#include <string.h>

#include "my-db/tables/firm.h"

int main() {
    firm_t *firm = createFirm("MyFirm", "I", 10, 100);

    assert(firm != NULL);
    assert(firm->id == -1);
    assert(firm->name != NULL);
    assert(strcmp(firm->name, "MyFirm") == 0);
    assert(firm->workers != NULL);
    assert(strcmp(firm->workers, "I") == 0);
    assert(firm->price == 10);
    assert(firm->monthTax == 100);

    freeFirm(firm);
}