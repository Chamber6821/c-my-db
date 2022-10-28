#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"
#include "my-db/request.h"

int main() {
    FILE *file = tmpfile();
    fputs("[AddRequest]\n", file);
    fputs("scheme=name|cash|pet\n", file);
    fputs("records[]=\"John|100.0|cat\"\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *requestSection = findSection(config, "AddRequest");
    AddRequest *request = tryBuildAddRequest(requestSection);

    assert(request != NULL);
    assert(request->base.type == RT_ADD);
    assert(strcmp(request->base.name, "AddRequest") == 0);
    assert(vector_size(request->records) == 1);

    Record record = request->records[0];
    assert(record != NULL);
    assert(vector_size(record) == 3);

    assert(record[0] != NULL);
    assert(getString(record[0]) != NULL);
    assert(strcmp(record[0]->name, "name") == 0);
    assert(strcmp(getString(record[0]), "John") == 0);

    assert(record[1] != NULL);
    assert(strcmp(record[1]->name, "cash") == 0);
    assert(getNumber(record[1]) == 100.f);

    assert(record[2] != NULL);
    assert(getString(record[2]) != NULL);
    assert(strcmp(record[2]->name, "pet") == 0);
    assert(strcmp(getString(record[2]), "cat") == 0);
}