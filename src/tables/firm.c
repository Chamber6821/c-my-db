#include <malloc.h>

#include "my-db/tables/firm.h"

firm_t *createDefaultFirm() {
    firm_t *new = malloc(sizeof(firm_t));
    new->name = sdsempty();
    new->workers = sdsempty();
    new->price = 0;
    new->monthTax = 0;
    return new;
}

void freeFirm(firm_t *this) {}
