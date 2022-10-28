#pragma once

#include <stdbool.h>

#include "my-db/ini/Property.h"
#include "sds/sds.h"

bool sdsstartwith(sds str, const char *prefix);

vec_sds sdssplit(const char *s, const char *sep);
