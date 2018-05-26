/*
 * evtx_header.c
 *
 * Copyright (c) 2018, Mark Weiman <mark.weiman@markzz.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "evtx_header.h"
#include "evtx_chnk_header.h"
#include "util.h"

struct _evtx_t {
    long first_chunk;
    long last_chunk;
    long next_rec_id;
    int size;
    int blk_size;
    unsigned int num_chunks;
    int flags;
    unsigned int checksum;

    evtx_chnk_header_t **chunks;
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

void evtx_header_free(evtx_t *h) {
    /* there's nothing really complicated with this struct */
    FREE(h);
}

evtx_t *evtx_header_init(char *bytes) {
    evtx_t *ret = NULL;
    int pos = 4096;

    CALLOC(ret, 1, sizeof(evtx_t), return NULL);

    if (_check_first_eight(bytes) != 0) {
        /* TODO: Log failure */
        evtx_header_free(ret);
        return NULL;
    }

    ret->first_chunk = eight_bytes_to_int64(bytes + 0x08);
    ret->last_chunk = eight_bytes_to_int64(bytes + 0x10);
    ret->next_rec_id = eight_bytes_to_int64(bytes + 0x18);

    if (four_bytes_to_int32(bytes + 0x20) != 0x80) {
        /* TODO: Display warning */
    }

    if (two_bytes_to_int16(bytes + 0x24) != 3) {
        /* TODO: Display warning */
    }

    if (two_bytes_to_int16(bytes + 0x28) != 1) {
        /* TODO: Display warning */
    }

    ret->num_chunks = two_bytes_to_int16(bytes + 0x2A);
    ret->flags = four_bytes_to_int32(bytes + 0x78);
    ret->checksum = (unsigned int) four_bytes_to_int32(bytes + 0x7C);

    CALLOC(ret->chunks, (size_t)ret->num_chunks, sizeof(evtx_chnk_header_t*), return NULL);


    for (int i = 0; i < ret->num_chunks; i++) {
        pos += evtx_chnk_header_init(ret->chunks + i, bytes + pos);

        while (strcmp(bytes + pos, "ElfChnk") != 0) {
            pos++;
        }
    }

    /* TODO: Check crc32 value */
    return ret;
}