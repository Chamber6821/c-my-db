#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"
#include "my-db/request.h"

int main() {
    FILE *file = tmpfile();
    fputs("[RequestWithWhere]\n", file);
    fputs("select[]=name\n", file);
    fputs("select[]=tax\n", file);
    fputs("whereField=id\n", file);
    fputs("whereCond=\"<=\"\n", file);
    fputs("whereOperand=\"5\"\n", file);
    rewind(file);

    Config *config = readConfigFile(file);

    Section *requestSection = findSection(config, "RequestWithWhere");
    SelectRequest *request = tryBuildSelectRequest(requestSection);

    assert(request != NULL);
    assert(strcmp(request->base.name, "RequestWithWhere") == 0);
    assert(vector_size(request->selectFields) == 2);
    assert(strcmp(request->selectFields[0], "name") == 0);
    assert(strcmp(request->selectFields[1], "tax") == 0);
    assert(request->whereRule != NULL);
    assert(strcmp(request->whereRule->field, "id") == 0);
    assert(request->whereRule->condition == (C_LT | C_EQ));
    assert(strcmp(request->whereRule->operand, "5") == 0);
    assert(request->orderRule == NULL);
    assert(request->limit == 0);
}