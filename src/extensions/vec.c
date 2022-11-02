#include "extensions/vec.h"

#include <memory.h>
#include <stddef.h>

void vector_remove_nulls(vec_pointer vec) {
    for (int i = vector_size(vec) - 1; i >= 0; i--) {
        if (vec[i] == NULL) vector_remove(&vec, i);
    }
}

void _vector_reverse(vector vec, int elementSize) {
    char *vec_ch = vec;
    int size = vector_size(vec);
    char buffer[elementSize];
    for (int i = 0; i < size / 2; i++) {
        int leftOffset = i * elementSize;
        int rightOffset = (size - 1 - i) * elementSize;

        memcpy(buffer, &vec_ch[leftOffset], elementSize);
        memcpy(&vec_ch[leftOffset], &vec_ch[rightOffset], elementSize);
        memcpy(&vec_ch[rightOffset], buffer, elementSize);
    }
}
