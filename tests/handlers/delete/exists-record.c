#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/Database.h"
#include "my-db/handlers.h"
#include "my-db/ini/reader.h"
#include "my-db/request.h"

int main() {
    Database *db = createEmptyDatabase();
    firm_t *firm1, *firm2;
    addRecord(db, firm1 = createFirm("BonGo", "He,She,They,Them", 0, 1e6));
    addRecord(db, createFirm("MoneyGen", "Me", 86, 1e6));
    addRecord(db, firm2 = createFirm("LoveKva", "Sans,Sans", -40, 8));

    Config *config = readConfigString(
        "[DeleteRequest]\n"
        "delete=all\n"
        "whereField=name\n"
        "whereCond=\"=\"\n"
        "whereOperand=MoneyGen\n");

    Section *requestSection = findSection(config, "DeleteRequest");
    DeleteRequest *request = tryBuildDeleteRequest(requestSection);

    Section *result = handleDeleteRequest(db, request);

    assert(result != NULL);
    assert(strcmp(result->name, "DeleteRequest") == 0);

    Property *deletedProp = findProperty(result, "deleted");
    assert(deletedProp != NULL);
    assert(deletedProp->type == PT_NUMBER);
    assert(getNumber(deletedProp) == 1);

    assert(vector_size(db->firms) == 3);
    assert(db->firms[0] == firm1);
    assert(db->firms[1] == NULL);
    assert(db->firms[2] == firm2);

    freeSection(result);
    freeConfig(config);
    freeRequest(&request->base);
    freeDatabase(db);
}