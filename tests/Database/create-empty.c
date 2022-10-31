#include <assert.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();

    assert(db != NULL);
    assert(db->firms != NULL);
    assert(vector_size(db->firms) == 0);

    freeDatabase(db);
}