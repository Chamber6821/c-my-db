#pragma once

#include "sds/sds.h"

typedef struct firm firm_t;
struct firm {
    int id;
    sds name;
    sds workers;
    float price;
    float monthTax;
};

firm_t *createDefaultFirm();
firm_t *createFirm(const char *name, const char *workers, float price,
                   float monthTax);
void freeFirm(firm_t *this);
