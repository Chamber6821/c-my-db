#pragma once

#include "c-vector/vec.h"

#define vector_deep_free(vec, elementDestructor, counter)              \
    ({                                                                 \
        for (int counter = 0; counter < vector_size(vec); counter++) { \
            (elementDestructor)((vec)[counter]);                       \
        }                                                              \
        vector_free(vec);                                              \
    })

typedef void **vec_pointer;

void vector_remove_nulls(vec_pointer vec);
