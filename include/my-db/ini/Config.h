#pragma once

#include "Section.h"

typedef Section **vec_Section;

typedef struct {
    vec_Section sections;
} Config;

Config *createEmptyConfig();
void freeConfig(Config *this);

Section *addSection(Config *this, const char *sectionName);

Section *findSection(Config *this, const char *sectionName);
