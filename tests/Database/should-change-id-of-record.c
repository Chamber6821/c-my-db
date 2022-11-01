#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();
    addRecord(db, createDefaultFirm());
    addRecord(db, createDefaultFirm());
    addRecord(db, createDefaultFirm());

    assert(db->firms != NULL);
    assert(vector_size(db->firms) == 3);

    assert(db->firms[0] != NULL);
    assert(db->firms[1] != NULL);
    assert(db->firms[2] != NULL);

    assert(db->firms[0]->id == 0);
    assert(db->firms[1]->id == 1);
    assert(db->firms[2]->id == 2);

    freeDatabase(db);
}