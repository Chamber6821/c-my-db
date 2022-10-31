#include "my-db/handlers.h"

#include <string.h>

#include "c-vector/vec.h"

static firm_t *recordToFirm(Record record) {
    firm_t *firm = createDefaultFirm();
    for (int i = 0; i < vector_size(record); i++) {
        Property *prop = record[i];

        if (strcmp(prop->name, "name") == 0) {
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

Section *handleAddRequest(Database *db, AddRequest *request) {
    for (int i = 0; i < vector_size(request->records); i++) {
        vector_add(&db->firms, recordToFirm(request->records[i]));
    }

    Property *added = createEmptyProperty("added");
    setNumber(added, vector_size(request->records));

    Section *result = createEmptySection(request->base.name);
    vector_add(&result->properties, added);

    return result;
}
