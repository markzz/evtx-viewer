//
// Created by markzz on 5/4/18.
//

#include <stdlib.h>

#include "evtx_header.h"
#include "util.h"

struct _evtx_header_t {
    long first_chunk;
    long last_chunk;
    long next_rec_id;
    int size;
    int blk_size;
    int num_chunks;
    int flags;
    unsigned int checksum;
};

static int _check_first_eight(const char *bytes) {
    char expected[] = {'E', 'l', 'f', 'F', 'i', 'l', 'e', 0};
    for (int i = 0; i < 8; i++) {
        if (bytes[i] != expected[i]) {
            return -1;
        }
    }
    return 0;
}

void evtx_header_free(evtx_header_t *h) {
    /* there's nothing really complicated with this struct */
    FREE(h);
}

evtx_header_t *evtx_header_init(char *bytes) {
    evtx_header_t *ret = NULL;

    CALLOC(ret, 1, sizeof(evtx_header_t), return NULL);

    if (_check_first_eight(bytes) != 0) {
        /* TODO: Log failure */
        evtx_header_free(ret);
        return NULL;
    }

    ret->first_chunk = eight_byes_to_long(bytes + 0x08);
    ret->last_chunk = eight_byes_to_long(bytes + 0x10);
    ret->next_rec_id = eight_byes_to_long(bytes + 0x18);

    if (four_bytes_to_int(bytes + 0x20) != 0x80) {
        /* TODO: Display warning */
    }

    if (two_bytes_to_int(bytes + 0x24) != 3) {
        /* TODO: Display warning */
    }

    if (two_bytes_to_int(bytes + 0x28) != 1) {
        /* TODO: Display warning */
    }

    ret->num_chunks = two_bytes_to_int(bytes + 0x2A);
    ret->flags = four_bytes_to_int(bytes + 0x78);
    ret->checksum = (unsigned int)four_bytes_to_int(bytes + 0x7C);

    /* TODO: Check crc32 value */
    return ret;
}