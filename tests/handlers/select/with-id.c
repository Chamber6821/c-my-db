#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"
#include "my-db/handlers.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"
#include "my-db/request.h"

int main() {
    Database *db = createEmptyDatabase();
    addRecord(db, createFirm("BonGo", "He,She,They,Them", 0, 1e6));
    addRecord(db, createFirm("MoneyGen", "Me", 86, 1e6));
    addRecord(db, createFirm("LoveKva", "Sans,Sans", -40, 8));

    Config *config = readConfigString(
        "[SelectRequest]\n"
        "select[]=id\n"
        "select[]=name\n");

    Section *requestSection = findSection(config, "SelectRequest");
    SelectRequest *request = tryBuildSelectRequest(requestSection);

    Section *result = handleSelectRequest(db, request);

    assert(result != NULL);
    assert(strcmp(result->name, "SelectRequest") == 0);

    Property *recordsProp = findProperty(result, "records[]");
    assert(recordsProp != NULL);

    vec_sds records = getStrings(recordsProp);
    assert(records != NULL);
    assert(vector_size(records) == 3);

    assert(strcmp(records[0], "0|BonGo") == 0);
    assert(strcmp(records[1], "1|MoneyGen") == 0);
    assert(strcmp(records[2], "2|LoveKva") == 0);

    freeConfig(config);
    freeRequest(&request->base);
    freeDatabase(db);
}