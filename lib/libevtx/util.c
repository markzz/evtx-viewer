//
// Created by markzz on 5/4/18.
//

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void _evtx_alloc_fail(size_t size) {
    fprintf(stderr, "allocation failure: could not allocate %zu bytes\n", size);
}

int two_bytes_to_int(const char *bytes) {
    return bytes[0] | bytes[1] << 8;
}

int four_bytes_to_int(const char *bytes) {
    return two_bytes_to_int(bytes) | bytes[2] << 16 | bytes[3] << 24;
}

long eight_byes_to_long(const char *bytes) {
    return (long)four_bytes_to_int(bytes) | bytes[4] << 32 | bytes[5] << 40 |
           bytes[6] << 48 | bytes[7] << 56;
}