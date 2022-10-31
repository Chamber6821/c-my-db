#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/handlers.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"

int main() {
    Database *db = createEmptyDatabase();
    assert(db != NULL);

    FILE *file = tmpfile();
    fputs("[AddRequest]\n", file);
    fputs("scheme=name|price|monthTax\n", file);
    fputs("records[]=\"FutureCorp|123e3|1e9\"\n", file);
    fputs("records[]=\"TimesNew|666e2|1e9\"\n", file);
    rewind(file);

    Config *config = readConfigFile(file);
    assert(config != NULL);
    AddRequest *request = tryBuildAddRequest(findSection(config, "AddRequest"));
    assert(request != NULL);

    // result Section used in another test
    freeSection(handleAddRequest(db, request));

    assert(vector_size(db->firms) == 2);

    firm_t *firm = db->firms[0];
    assert(firm != NULL);
    assert(strcmp(firm->name, "FutureCorp") == 0);
    assert(strcmp(firm->workers, "") == 0);
    assert(firm->price == 123e3);
    assert(firm->monthTax == 1e9);

    firm = db->firms[1];
    assert(firm != NULL);
    assert(strcmp(firm->name, "TimesNew") == 0);
    assert(strcmp(firm->workers, "") == 0);
    assert(firm->price == 666e2);
    assert(firm->monthTax == 1e9);

    freeRequest(&request->base);
    freeConfig(config);
    freeDatabase(db);
}