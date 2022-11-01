#include "my-db/tables/firm.h"

#include <malloc.h>

firm_t *createDefaultFirm() { return createFirm(NULL, NULL, 0, 0); }

firm_t *createFirm(const char *name, const char *workers, float price,
                   float monthTax) {
    firm_t *new = malloc(sizeof(firm_t));
    new->id = -1;
    new->name = name ? sdsnew(name) : sdsempty();
    new->workers = workers ? sdsnew(workers) : sdsempty();
    new->price = price;
    new->monthTax = monthTax;
}

void freeFirm(firm_t *this) {
    if (this == NULL) return;
    sdsfree(this->name);
    sdsfree(this->workers);
    free(this);
}
