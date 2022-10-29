#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"
#include "my-db/request.h"

int main() {
    FILE *file = tmpfile();
    fputs("[RequestWithDefaultOrder]\n", file);
    fputs("select[]=name\n", file);
    fputs("select[]=tax\n", file);
    fputs("orderBy=name\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *requestSection = findSection(config, "RequestWithDefaultOrder");
    SelectRequest *request = tryBuildSelectRequest(requestSection);

    assert(request != NULL);
    assert(strcmp(request->base.name, "RequestWithDefaultOrder") == 0);
    assert(vector_size(request->selectFields) == 2);
    assert(strcmp(request->selectFields[0], "name") == 0);
    assert(strcmp(request->selectFields[1], "tax") == 0);
    assert(request->whereRule == NULL);
    assert(request->orderRule != NULL);
    assert(strcmp(request->orderRule->field, "name") == 0);
    assert(request->orderRule->direction == RO_ASC);
    assert(request->limit == 0);

    freeConfig(config);
    freeRequest(&request->base);
}