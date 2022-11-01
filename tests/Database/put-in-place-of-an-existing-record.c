#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();

    firm_t *firm1 = createDefaultFirm();
    firm_t *firm2 = createDefaultFirm();
    addRecord(db, firm1);
    addRecord(db, firm2);

    firm_t *firm3 = createDefaultFirm();
    bool status = tryPutRecordWithId(db, 0, firm3);

    assert(db->firms != NULL);
    assert(vector_size(db->firms) == 2);
    assert(db->firms[0]->id == 0);
    assert(db->firms[0]->firm == firm1);
    assert(db->firms[1]->id == 1);
    assert(db->firms[1]->firm == firm2);
    assert(status == false);

    freeFirm(firm3);
    freeDatabase(db);
}