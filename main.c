#include "c-vector/vec.h"
#include "my-db/Database.h"
#include "my-db/handlers.h"
#include "my-db/ini/Config.h"
#include "my-db/ini/reader.h"
#include "my-db/ini/writer.h"
#include "my-db/request.h"

int main() {
    Database *db = createEmptyDatabase();

    // все популярные ОСи поддерживают перенаправление потока ввода из файла
    Config *allRequests = readConfigFile(stdin);

    for (int i = 0; i < vector_size(allRequests->sections); i++) {
        Request *request = tryBuildRequest(allRequests->sections[i]);
        Section *result = handleRequest(db, request);

        // все популярные ОСи поддерживают перенаправление потока вывода в файл
        dumpSectionToFile(stdout, result);

        freeSection(result);
        freeRequest(request);
    }

    freeDatabase(db);
}
