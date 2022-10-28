#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"
#include "my-db/request.h"

int main() {
    FILE *file = tmpfile();
    fputs("[FirstSelectRequest]\n", file);
    fputs("select[]=id\n", file);
    fputs("select[]=companyName\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *requestSection = findSection(config, "FirstSelectRequest");
    SelectRequest *request = tryBuildSelectRequest(requestSection);

    assert(request != NULL);
    assert(strcmp(request->base.name, "FirstSelectRequest") == 0);
    assert(vector_size(request->selectFields) == 2);
    assert(strcmp(request->selectFields[0], "id") == 0);
    assert(strcmp(request->selectFields[1], "companyName") == 0);
    assert(request->whereRule == NULL);
    assert(request->orderRule == NULL);
    assert(request->limit == 0);
}