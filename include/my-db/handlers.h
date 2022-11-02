#pragma once

#include "Database.h"
#include "my-db/ini/Section.h"
#include "request.h"

//Section *handleRequest(Database *db, Request *request);

Section *handleSelectRequest(Database *db, SelectRequest *request);
Section *handleAddRequest(Database *db, AddRequest *request);
//Section *handleDeleteRequest(Database *db, DeleteRequest *request);
