#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "Config.h"

bool dumpConfigToFile(FILE *file, Config *config);

bool dumpSectionToFile(FILE *file, Section *section);
