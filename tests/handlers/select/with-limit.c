#include <assert.h>
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
        "[RequestWithLimit]\n"
        "select[]=name\n"
        "select[]=monthTax\n"
        "orderBy=name\n"
        "limit=2");

    Section *requestSection = findSection(config, "RequestWithLimit");
    SelectRequest *request = tryBuildSelectRequest(requestSection);

    Section *result = handleSelectRequest(db, request);

    assert(result != NULL);
    assert(strcmp(result->name, "RequestWithLimit") == 0);

    Property *recordsProp = findProperty(result, "records[]");
    assert(recordsProp != NULL);

    vec_sds records = getStrings(recordsProp);
    assert(records != NULL);
    assert(vector_size(records) == 2);

    assert(strcmp(records[0], "BonGo|1e+06") == 0);
    assert(strcmp(records[1], "LoveKva|8") == 0);

    freeSection(result);
    freeConfig(config);
    freeRequest(&request->base);
    freeDatabase(db);
}