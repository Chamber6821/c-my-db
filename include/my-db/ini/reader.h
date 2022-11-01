#pragma once

#include <stdio.h>

#include "Config.h"

Config *readConfigFile(FILE *source);

Config *readConfigString(const char *source);
