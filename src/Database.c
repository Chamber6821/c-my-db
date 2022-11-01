#include "my-db/Database.h"

#include <malloc.h>

#include "c-vector/vec.h"
#include "extensions/vec.h"

static inline FirmRow *createRow(int id, firm_t *firm) {
    if (firm == NULL) return NULL;
    FirmRow *new = malloc(sizeof(FirmRow));
    new->id = id;
    new->firm = firm;
    return new;
}

static inline void freeRow(FirmRow *row) {
    if (row == NULL) return;
    freeFirm(row->firm);
    free(row);
}

Database *createEmptyDatabase() {
    Database *new = malloc(sizeof(Database));
    new->firms = vector_create();
    return new;
}

void freeDatabase(Database *this) {
    if (this == NULL) return;
    vector_deep_free(this->firms, freeRow, i);
    free(this);
}

void addRecord(Database *this, firm_t *record) {
    vector_add(&this->firms, createRow(vector_size(this->firms), record));
}

bool tryPutRecordWithId(Database *this, int id, firm_t *record) {
    if (id < 0) return false;

    while (vector_size(this->firms) <= id) vector_add(&this->firms, NULL);

    if (this->firms[id] != NULL) return false;

    this->firms[id] = createRow(id, record);
    return true;
}

void deleteRecord(Database *this, int id) {
    if (0 > id || id >= vector_size(this->firms)) return;

    if (this->firms[id] != NULL) freeRow(this->firms[id]);
    this->firms[id] = NULL;
}
