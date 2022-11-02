#include <stddef.h>

#include "my-db/Database.h"

int main() {
    Database *db = createEmptyDatabase();

    addRecord(db, NULL);

    freeDatabase(db);
}