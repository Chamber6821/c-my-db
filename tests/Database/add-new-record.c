#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();

    firm_t *firm = createDefaultFirm();
    addRecord(db, firm);

    assert(db->firms != NULL);
    assert(vector_size(db->firms) == 1);
    assert(db->firms[0] == firm);

    freeDatabase(db);
}