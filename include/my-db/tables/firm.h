#pragma once

#include "sds/sds.h"

typedef struct firm firm_t;
struct firm {
    sds name;
    sds workers;
    float price;
    float monthTax;
};

firm_t *createDefaultFirm();
void freeFirm(firm_t *this);
