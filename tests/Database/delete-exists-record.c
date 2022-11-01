#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();

    firm_t *firm1 = createDefaultFirm();
    firm_t *firm2 = createDefaultFirm();
    addRecord(db, firm1);
    addRecord(db, firm2);

    deleteRecord(db, 0);

    assert(db->firms != NULL);
    assert(vector_size(db->firms) == 2);
    assert(db->firms[0] == NULL);
    assert(db->firms[1] == firm2);

    freeDatabase(db);
}