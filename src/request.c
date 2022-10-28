#include "my-db/request.h"

#include <malloc.h>
#include <string.h>

static Condition toCondition(const char* str) {
    if (str == NULL) return C_NONE;
    if (strcmp(str, "<=") == 0) return C_LT | C_EQ;
    if (strcmp(str, ">=") == 0) return C_GT | C_EQ;
    if (strcmp(str, "=") == 0) return C_EQ;
    if (strcmp(str, "<") == 0) return C_LT;
    if (strcmp(str, ">") == 0) return C_GT;
    return C_NONE;
}

static OrderDirection toDirection(const char* lower) {
    if (strcmp(lower, "asc") == 0) return RO_ASC;
    if (strcmp(lower, "desc") == 0) return RO_DESC;
    return RO_ASC;
}

static WhereRule* tryBuildWhereRule(Section* section) {
    Property* property = findProperty(section, "whereField");
    if (property == NULL) return NULL;
    sds field = getString(property);
    if (field == NULL) return NULL;

    property = findProperty(section, "whereCond");
    if (property == NULL) return NULL;
    Condition cond = toCondition(getString(property));
    if (cond == C_NONE) return NULL;

    property = findProperty(section, "whereOperand");
    if (property == NULL) return NULL;
    sds operand = getString(property);
    if (operand == NULL) return NULL;

    WhereRule* rule = malloc(sizeof(WhereRule));
    rule->field = field;
    rule->condition = cond;
    rule->operand = operand;
    return rule;
}

static OrderRule* tryBuildOrderRule(Section* section) {
    Property* property = findProperty(section, "orderBy");
    if (property == NULL) return NULL;
    sds field = getString(property);
    if (field == NULL) return NULL;

    property = findProperty(section, "orderDir");
    sds dirStr = (property == NULL) ? sdsempty() : sdsdup(getString(property));
    sdstolower(dirStr);
    OrderDirection dir = toDirection(dirStr);
    sdsfree(dirStr);

    OrderRule* rule = malloc(sizeof(OrderRule));
    rule->field = field;
    rule->direction = dir;
    return rule;
}

Request* tryBuildRequest(Section* section) { return NULL; }

void freeRequest(Request* request) {}

SelectRequest* tryBuildSelectRequest(Section* section) {
    Property* property = findProperty(section, "select[]");
    if (property == NULL) return NULL;
    vec_sds fields = getStrings(property);
    if (fields == NULL) return NULL;

    property = findProperty(section, "limit");
    float limit = 0;
    if (property != NULL) limit = getNumber(property);

    SelectRequest* request = malloc(sizeof(SelectRequest));
    request->base.type = RT_SELECT;
    request->base.name = section->name;
    request->selectFields = fields;
    request->whereRule = tryBuildWhereRule(section);
    request->orderRule = tryBuildOrderRule(section);
    request->limit = limit;
    return request;
}

AddRequest* tryBuildAddRequest(Section* section) { return NULL; }

DeleteRequest* tryBuildDeleteRequest(Section* section) { return NULL; }
