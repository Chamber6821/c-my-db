#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();

    firm_t *firm2 = createDefaultFirm();
    addRecord(db, NULL);
    addRecord(db, firm2);

    firm_t *firm3 = createDefaultFirm();
    bool status = tryPutRecordWithId(db, 0, firm3);

    assert(db->firms != NULL);
    assert(vector_size(db->firms) == 2);
    assert(db->firms[0] == firm3);
    assert(db->firms[1] == firm2);
    assert(status == true);

    freeDatabase(db);
}