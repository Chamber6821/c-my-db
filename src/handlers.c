#include "my-db/handlers.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "c-vector/vec.h"
#include "extensions/vec.h"

#define NUMBER_COMPARATOR(struct, field)                                    \
    int compare_##struct##_##field(const void *a, const void *b) {          \
        return (*(const struct **)a)->field - (*(const struct **)b)->field; \
    }

#define STRING_COMPARATOR(struct, field)                           \
    int compare_##struct##_##field(const void *a, const void *b) { \
        return strcmp((*(const struct **)a)->field,                \
                      (*(const struct **)b)->field);               \
    }

NUMBER_COMPARATOR(firm_t, id)
STRING_COMPARATOR(firm_t, name)
STRING_COMPARATOR(firm_t, workers)
NUMBER_COMPARATOR(firm_t, price)
NUMBER_COMPARATOR(firm_t, monthTax)

typedef int (*Comparator)(const void *a, const void *b);

int alwaysZero(const void *a, const void *b) { return 0; }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
bool compare(float left, Condition cond, float right) {
    switch (cond) {
        case C_NONE:
            return false;
        case C_GT:
            return left > right;
        case C_LT:
            return left < right;
        case C_EQ:
            return left == right;
        case C_NEQ:
            return left != right;
        case (C_GT | C_EQ):
            return left >= right;
        case (C_LT | C_EQ):
            return left <= right;
    }
}
#pragma clang diagnostic pop

static firm_t *recordToFirm(Record record) {
    firm_t *firm = createDefaultFirm();
    for (int i = 0; i < vector_size(record); i++) {
        Property *prop = record[i];

        if (strcmp(prop->name, "id") == 0) {
            if (prop->type == PT_NUMBER) {
                float value = getNumber(prop);
                firm->id = value;
            }
        }

        else if (strcmp(prop->name, "name") == 0) {
            if (prop->type == PT_STRING) {
                sds value = getString(prop);
                sdsfree(firm->name);
                firm->name = sdsdup(value);
            }
        }

        else if (strcmp(prop->name, "workers") == 0) {
            if (prop->type == PT_STRING) {
                sds value = getString(prop);
                sdsfree(firm->workers);
                firm->workers = sdsdup(value);
            }
        }

        else if (strcmp(prop->name, "price") == 0) {
            if (prop->type == PT_NUMBER) {
                float value = getNumber(prop);
                firm->price = value;
            }
        }

        else if (strcmp(prop->name, "monthTax") == 0) {
            if (prop->type == PT_NUMBER) {
                float value = getNumber(prop);
                firm->monthTax = value;
            }
        }
    }
    return firm;
}

static sds recordToSdsByScheme(vec_sds scheme, firm_t *firm) {
    sds acc = sdsempty();

    for (int i = 0; i < vector_size(scheme); i++) {
        sds field = scheme[i];

        if (strcmp(field, "id") == 0) {
            acc = sdscatprintf(acc, "%d", firm->id);
        } else if (strcmp(field, "name") == 0) {
            acc = sdscatsds(acc, firm->name);
        } else if (strcmp(field, "workers") == 0) {
            acc = sdscatsds(acc, firm->workers);
        } else if (strcmp(field, "price") == 0) {
            acc = sdscatprintf(acc, "%g", firm->price);
        } else if (strcmp(field, "monthTax") == 0) {
            acc = sdscatprintf(acc, "%g", firm->monthTax);
        }

        acc = sdscat(acc, "|");
    }

    sdsrange(acc, 0, -2);
    return acc;
}

Comparator getComparator(const char *field) {
    if (strcmp(field, "id") == 0) return compare_firm_t_id;
    if (strcmp(field, "name") == 0) return compare_firm_t_name;
    if (strcmp(field, "workers") == 0) return compare_firm_t_workers;
    if (strcmp(field, "price") == 0) return compare_firm_t_price;
    if (strcmp(field, "monthTax") == 0) return compare_firm_t_monthTax;
    return alwaysZero;
}

void sortByRule(firmsTable firms, OrderRule *rule) {
    if (rule == NULL) return;

    Comparator comparator = getComparator(rule->field);
    if (comparator == alwaysZero) return;

    qsort(firms, vector_size(firms), sizeof(firm_t *), comparator);

    if (rule->direction == RO_DESC) vector_reverse(firms);
}

bool isSuit(firm_t *firm, WhereRule *rule) {
    if (rule == NULL) return true;

    float operand = 0;
    int scanned = sscanf(rule->operand, "%f", &operand);

    if (strcmp(rule->field, "id") == 0) {
        if (scanned != 1) return false;
        return compare(firm->id, rule->condition, operand);
    } else if (strcmp(rule->field, "name") == 0) {
        return compare(strcmp(firm->name, rule->operand), rule->condition, 0);
    } else if (strcmp(rule->field, "workers") == 0) {
        return compare(strcmp(firm->workers, rule->operand), rule->condition,
                       0);
    } else if (strcmp(rule->field, "price") == 0) {
        if (scanned != 1) return false;
        return compare(firm->price, rule->condition, operand);
    } else if (strcmp(rule->field, "monthTax") == 0) {
        if (scanned != 1) return false;
        return compare(firm->monthTax, rule->condition, operand);
    }

    return false;
}

Section *handleSelectRequest(Database *db, SelectRequest *request) {
    vec_sds scheme = request->selectFields;
    firmsTable firms = vector_copy(db->firms);

    vector_remove_nulls((vec_pointer)firms);
    if (request->orderRule) sortByRule(firms, request->orderRule);

    int limit = request->limit ? request->limit : INT_MAX;

    int selected = 0;  // vector_size(...) is too unwieldy
    vec_sds convertedRecords = vector_create();
    for (int i = 0; i < vector_size(firms) && selected < limit; i++) {
        firm_t *firm = firms[i];
        if (firm == NULL) continue;
        if (!isSuit(firm, request->whereRule)) continue;

        vector_add(&convertedRecords, recordToSdsByScheme(scheme, firm));
        selected++;
    }

    Section *result = createEmptySection(request->base.name);
    copyToStrings(addEmptyProperty(result, "records[]"), convertedRecords);
    vector_free(convertedRecords);
    vector_free(firms);  // not deep free because I am not the owner of records

    return result;
}

Section *handleAddRequest(Database *db, AddRequest *request) {
    int added = 0;
    for (int i = 0; i < vector_size(request->records); i++) {
        firm_t *record = recordToFirm(request->records[i]);
        if (record->id == -1) {
            addRecord(db, record);
            added++;
        } else {
            added += tryPutRecordWithId(db, record->id, record);
        }
    }

    Section *result = createEmptySection(request->base.name);
    setNumber(addEmptyProperty(result, "added"), added);

    return result;
}
