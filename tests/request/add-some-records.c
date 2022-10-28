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
    fputs("scheme=character|danger|look\n", file);
    fputs("records[]=Pudge|very|mister galaxy\n", file);
    fputs("records[]=Terminator|baby|poof\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *requestSection = findSection(config, "AddRequest");
    AddRequest *request = tryBuildAddRequest(requestSection);

    assert(request != NULL);
    assert(request->base.type == RT_ADD);
    assert(strcmp(request->base.name, "AddRequest") == 0);
    assert(vector_size(request->records) == 2);

    Record record1 = request->records[0];
    assert(record1 != NULL);
    assert(vector_size(record1) == 3);

    assert(record1[0] != NULL);
    assert(getString(record1[0]) != NULL);
    assert(strcmp(record1[0]->name, "character") == 0);
    assert(strcmp(getString(record1[0]), "Pudge") == 0);

    assert(record1[1] != NULL);
    assert(getString(record1[1]) != NULL);
    assert(strcmp(record1[1]->name, "danger") == 0);
    assert(strcmp(getString(record1[1]), "very") == 0);

    assert(record1[2] != NULL);
    assert(getString(record1[2]) != NULL);
    assert(strcmp(record1[2]->name, "look") == 0);
    assert(strcmp(getString(record1[2]), "mister galaxy") == 0);

    Record record2 = request->records[1];
    assert(record2 != NULL);
    assert(vector_size(record2) == 3);

    assert(record2[0] != NULL);
    assert(getString(record2[0]) != NULL);
    assert(strcmp(record2[0]->name, "character") == 0);
    assert(strcmp(getString(record2[0]), "Terminator") == 0);

    assert(record2[1] != NULL);
    assert(getString(record2[1]) != NULL);
    assert(strcmp(record2[1]->name, "danger") == 0);
    assert(strcmp(getString(record2[1]), "baby") == 0);

    assert(record2[2] != NULL);
    assert(getString(record2[2]) != NULL);
    assert(strcmp(record2[2]->name, "look") == 0);
    assert(strcmp(getString(record2[2]), "poof") == 0);
}