#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();

    firm_t *firm1 = createDefaultFirm();
    addRecord(db, firm1);
    addRecord(db, NULL);

    deleteRecord(db, 10);

    assert(db->firms != NULL);
    assert(vector_size(db->firms) == 2);
    assert(db->firms[0]->id == 0);
    assert(db->firms[0]->firm == firm1);
    assert(db->firms[1] == NULL);

    freeDatabase(db);
}