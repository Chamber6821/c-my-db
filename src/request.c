#include "my-db/request.h"

#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "extensions/sds.h"
#include "extensions/string.h"
#include "extensions/vec.h"

#ifndef _countof
#define _countof(arr) (sizeof(arr) / sizeof(*arr))
#endif

static inline void freeSelectRequest(SelectRequest *request);
static inline void freeAddRequest(AddRequest *request);
static inline void freeDeleteRequest(DeleteRequest *request);

static Condition toCondition(const char *str) {
    if (str == NULL) return C_NONE;
    if (strcmp(str, "<=") == 0) return C_LT | C_EQ;
    if (strcmp(str, ">=") == 0) return C_GT | C_EQ;
    if (strcmp(str, "=") == 0) return C_EQ;
    if (strcmp(str, "<") == 0) return C_LT;
    if (strcmp(str, ">") == 0) return C_GT;
    return C_NONE;
}

static OrderDirection toDirection(const char *lower) {
    if (strcmp(lower, "asc") == 0) return RO_ASC;
    if (strcmp(lower, "desc") == 0) return RO_DESC;
    return RO_ASC;
}

static WhereRule *tryBuildWhereRule(Section *section) {
    Property *property = findProperty(section, "whereField");
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

    WhereRule *rule = malloc(sizeof(WhereRule));
    rule->field = field;
    rule->condition = cond;
    rule->operand = operand;
    return rule;
}

static inline void freeWhereRule(WhereRule *rule) { free(rule); }

static OrderRule *tryBuildOrderRule(Section *section) {
    Property *property = findProperty(section, "orderBy");
    if (property == NULL) return NULL;
    sds field = getString(property);
    if (field == NULL) return NULL;

    property = findProperty(section, "orderDir");
    sds dirStr = (property == NULL) ? sdsempty() : sdsdup(getString(property));
    sdstolower(dirStr);
    OrderDirection dir = toDirection(dirStr);
    sdsfree(dirStr);

    OrderRule *rule = malloc(sizeof(OrderRule));
    rule->field = field;
    rule->direction = dir;
    return rule;
}

static inline void freeOrderRule(OrderRule *rule) { free(rule); }

typedef Request *(*builder)(Section *section);
Request *tryBuildRequest(Section *section) {
    builder builders[] = {
        (builder)tryBuildSelectRequest,
        (builder)tryBuildAddRequest,
        (builder)tryBuildDeleteRequest,
    };

    for (int i = 0; i < _countof(builders); i++) {
        Request *request = builders[i](section);
        if (request != NULL) return request;
    }
    return NULL;
}

typedef void (*destructor)(Request *request);
static inline destructor getDestructorFor(RequestType type) {
    switch (type) {
        case RT_SELECT:
            return (destructor)freeSelectRequest;
        case RT_ADD:
            return (destructor)freeAddRequest;
        case RT_DELETE:
            return (destructor)freeDeleteRequest;
    }
}

void freeRequest(Request *request) {
    if (request == NULL) return;
    getDestructorFor(request->type)(request);
}

Record newRecord() { return vector_create(); }

static inline void freeRecord(Record record) {
    vector_deep_free(record, freeProperty, i);
}

Record makeRecord(vec_sds scheme, vec_sds rawFields) {
    Record record = newRecord();
    for (int i = 0; i < vector_size(scheme); i++) {
        Property *prop = createEmptyProperty(scheme[i]);

        float number;
        if (1 == sscanf(rawFields[i], "%f", &number)) {
            setNumber(prop, number);
        } else {
            copyToString(prop, rawFields[i]);
        }

        vector_add(&record, prop);
    }
    return record;
}

SelectRequest *tryBuildSelectRequest(Section *section) {
    Property *property = findProperty(section, "select[]");
    if (property == NULL) return NULL;
    vec_sds fields = getStrings(property);
    if (fields == NULL) return NULL;

    property = findProperty(section, "limit");
    float limit = 0;
    if (property != NULL) limit = getNumber(property);

    SelectRequest *request = malloc(sizeof(SelectRequest));
    request->base.type = RT_SELECT;
    request->base.name = section->name;
    request->selectFields = fields;
    request->whereRule = tryBuildWhereRule(section);
    request->orderRule = tryBuildOrderRule(section);
    request->limit = limit;
    return request;
}

void freeSelectRequest(SelectRequest *request) {
    if (request == NULL) return;
    freeWhereRule(request->whereRule);
    freeOrderRule(request->orderRule);
    free(request);
}

AddRequest *tryBuildAddRequest(Section *section) {
    Property *schemeProp = findProperty(section, "scheme");
    if (schemeProp == NULL) return NULL;
    const char *gluedScheme = getString(schemeProp);
    if (gluedScheme == NULL) return NULL;

    Property *recordsProp = findProperty(section, "records[]");
    if (recordsProp == NULL) return NULL;
    vec_sds recordStrings = getStrings(recordsProp);
    if (recordStrings == NULL) return NULL;

    vec_sds scheme = sdssplit(gluedScheme, "|");
    if (scheme == NULL) return NULL;

    vec_Record records = vector_create();
    for (int i = 0; i < vector_size(recordStrings); i++) {
        int fieldCount = 1 + strstrcount(recordStrings[i], "|");
        if (vector_size(scheme) != fieldCount) continue;

        vec_sds rawFields = sdssplit(recordStrings[i], "|");
        vector_add(&records, makeRecord(scheme, rawFields));
        vector_deep_free(rawFields, sdsfree, j);
    }

    vector_deep_free(scheme, sdsfree, i);

    AddRequest *request = malloc(sizeof(AddRequest));
    request->base.type = RT_ADD;
    request->base.name = section->name;
    request->records = records;
    return request;
}

void freeAddRequest(AddRequest *request) {
    if (request == NULL) return;
    vector_deep_free(request->records, freeRecord, i);
    free(request);
}

DeleteRequest *tryBuildDeleteRequest(Section *section) {
    Property *property = findProperty(section, "delete");
    if (property == NULL) return NULL;
    sds flag = getString(property);
    if (flag == NULL || strcmp(flag, "all") != 0) return NULL;

    WhereRule *rule = tryBuildWhereRule(section);
    if (rule == NULL) return NULL;

    DeleteRequest *request = malloc(sizeof(DeleteRequest));
    request->base.type = RT_DELETE;
    request->base.name = section->name;
    request->whereRule = rule;
    return request;
}

void freeDeleteRequest(DeleteRequest *request) {
    if (request == NULL) return;
    freeWhereRule(request->whereRule);
    free(request);
}
