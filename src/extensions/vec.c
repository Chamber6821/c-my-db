#include "extensions/vec.h"

#include <stddef.h>

void vector_remove_nulls(vec_pointer vec) {
    for (int i = vector_size(vec) - 1; i >= 0; i--) {
        if (vec[i] == NULL) vector_remove(vec, i);
    }
}
