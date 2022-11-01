#pragma once

#include <stdbool.h>

#include "my-db/tables/firm.h"

typedef struct {
    int id;
    firm_t *firm;
} FirmRow;

typedef FirmRow **vec_FirmRow;

typedef struct {
    vec_FirmRow firms;
} Database;

Database *createEmptyDatabase();
void freeDatabase(Database *this);

void addRecord(Database *this, firm_t *record);
bool tryPutRecordWithId(Database *this, int id, firm_t *record);
void deleteRecord(Database *this, int id);
