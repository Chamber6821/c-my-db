#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();

    firm_t *firm2 = createDefaultFirm();
    addRecord(db, NULL);
    addRecord(db, firm2);

    firm_t *firm3 = createDefaultFirm();
    bool status = tryPutRecordWithId(db, 5, firm3);

    assert(db->firms != NULL);
    assert(vector_size(db->firms) == 6);
    assert(db->firms[0] == NULL);
    assert(db->firms[1]->id == 1);
    assert(db->firms[1]->firm == firm2);
    assert(db->firms[2] == NULL);
    assert(db->firms[3] == NULL);
    assert(db->firms[4] == NULL);
    assert(db->firms[5]->id == 5);
    assert(db->firms[5]->firm == firm3);
    assert(status == true);

    freeDatabase(db);
}