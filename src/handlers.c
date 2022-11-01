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
