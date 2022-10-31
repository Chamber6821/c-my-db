#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "my-db/handlers.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    Database *db = createEmptyDatabase();

    FILE *file = tmpfile();
    fputs("[AddRequest]\n", file);
    fputs("scheme=name|price|monthTax\n", file);
    fputs("records[]=\"FutureCorp|123e3|1e9\"\n", file);
    fputs("records[]=\"TimesNew|666e2|1e9\"\n", file);
    rewind(file);

    Config *config = readConfigFile(file);
    AddRequest *request = tryBuildAddRequest(findSection(config, "AddRequest"));

    Section *result = handleAddRequest(db, request);

    assert(result != NULL);
    assert(strcmp(result->name, "AddRequest") == 0);
    Property *addedCount = findProperty(result, "added");
    assert(addedCount != NULL);
    assert(addedCount->type == PT_NUMBER);
    assert(getNumber(addedCount) == 2);

    freeSection(result);
    freeRequest(&request->base);
    freeConfig(config);
    freeDatabase(db);
}