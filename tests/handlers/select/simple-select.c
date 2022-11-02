#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"
#include "my-db/handlers.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"
#include "my-db/request.h"

int main() {
    Database *db = createEmptyDatabase();
    assert(db != NULL);
    addRecord(db, createFirm("BonGo", "He,She,They,Them", 0, 1e6));
    addRecord(db, createFirm("MoneyGen", "Me", 86, 1e6));
    addRecord(db, createFirm("LoveKva", "Sans,Sans", -40, 8));

    FILE *file = tmpfile();
    fputs("[SelectRequest]\n", file);
    fputs("select[]=name\n", file);
    fputs("select[]=monthTax\n", file);
    rewind(file);

    Config *config = readConfigFile(file);
    assert(config != NULL);

    Section *requestSection = findSection(config, "SelectRequest");
    SelectRequest *request = tryBuildSelectRequest(requestSection);
    assert(request != NULL);

    Section *result = handleSelectRequest(db, request);

    assert(result != NULL);
    assert(strcmp(result->name, "SelectRequest") == 0);

    Property *recordsProp = findProperty(result, "records[]");
    assert(recordsProp != NULL);

    vec_sds records = getStrings(recordsProp);
    assert(records != NULL);
    assert(vector_size(records) == 3);

    assert(strcmp(records[0], "BonGo|1e+06") == 0);
    assert(strcmp(records[1], "MoneyGen|1e+06") == 0);
    assert(strcmp(records[2], "LoveKva|8") == 0);

    freeConfig(config);
    freeRequest(&request->base);
    freeDatabase(db);
}