#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"
#include "my-db/request.h"

int main() {
    FILE *file = tmpfile();
    fputs("[DeleteRequest]\n", file);
    fputs("delete=all\n", file);
    fputs("whereField=cash\n", file);
    fputs("whereCond=\"<\"\n", file);
    fputs("whereOperand=\"100.0\"\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *requestSection = findSection(config, "DeleteRequest");
    DeleteRequest *request = tryBuildDeleteRequest(requestSection);

    assert(request != NULL);
    assert(strcmp(request->base.name, "DeleteRequest") == 0);
    assert(request->whereRule != NULL);
    assert(strcmp(request->whereRule->field, "cash") == 0);
    assert(request->whereRule->condition == C_LT);
    assert(strcmp(request->whereRule->operand, "100.0") == 0);
}