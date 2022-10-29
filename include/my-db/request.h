#pragma once

#include "my-db/ini/Property.h"
#include "my-db/ini/Section.h"

typedef vec_Property Record;
typedef Record *vec_Record;

typedef enum {
    RO_ASC,
    RO_DESC,
} OrderDirection;

typedef enum {
    C_NONE = 0,
    C_GT = 0b100,
    C_LT = 0b110,
    C_EQ = 0b001,
} Condition;

typedef enum {
    RT_SELECT,
    RT_ADD,
    RT_DELETE,
} RequestType;

typedef struct {
    const char *field;
    OrderDirection direction;
} OrderRule;

typedef struct {
    const char *field;
    Condition condition;
    const char *operand;
} WhereRule;

typedef struct {
    RequestType type;
    const char *name;
} Request;

typedef struct {
    Request base;
    vec_sds selectFields;
    WhereRule *whereRule;
    OrderRule *orderRule;
    int limit;
} SelectRequest;

typedef struct {
    Request base;
    vec_Record records;
} AddRequest;

typedef struct {
    Request base;
    WhereRule *whereRule;
} DeleteRequest;

// Request *tryBuildRequest(Section *section);
// void freeRequest(Request *request);

SelectRequest *tryBuildSelectRequest(Section *section);
AddRequest *tryBuildAddRequest(Section *section);
DeleteRequest *tryBuildDeleteRequest(Section *section);
