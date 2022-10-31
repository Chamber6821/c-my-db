#include "my-db/Database.h"

#include <malloc.h>

#include "c-vector/vec.h"

Database *createEmptyDatabase() {
    Database *new = malloc(sizeof(Database));
    new->firms = vector_create();
    return new;
}

void freeDatabase(Database *this) {}

void addRecord(Database *this, const firm_t *record) {
    vector_add(&this->firms, record);
}

bool tryPutRecordWithId(Database *this, int id, const firm_t *record) {
    if (id < 0) return false;

    while (vector_size(this->firms) <= id) vector_add(&this->firms, NULL);

    if (this->firms[id] != NULL) return false;

    this->firms[id] = record;
    return true;
}

void deleteRecord(Database *this, int id) {
    if (0 <= id && id < vector_size(this->firms)) this->firms[id] = NULL;
}
