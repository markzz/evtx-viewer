/*
 * evtx_chnk_header.c
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

#include "evtx_chnk_header.h"
#include "evtx_record.h"
#include "util.h"

struct _evtx_chnk_header_t {
    long first_event_record_num;
    long last_event_record_num;
    long first_event_record_id;
    long last_event_record_id;
    int last_event_record_offset;
    int free_space_offset;
    int event_record_data_crc32;
    int flags;
    int header_crc32;

    int num_records;
    evtx_record_t **records;
};

static int _check_first_eight(const unsigned char *bytes) {
    char expected[] = {'E', 'l', 'f', 'C', 'h', 'n', 'k', 0};
    for (int i = 0; i < 8; i++) {
        if (bytes[i] != expected[i]) {
            return -1;
        }
    }
    return 0;
}

static int _check_next(const unsigned char *bytes) {
    char expected[] = {0x0F, 0x01, 0x01, 0x00};
    for (int i = 0; i < 4; i++) {
        if (bytes[i] != expected[i]) {
            return -1;
        }
    }
    return 0;
}

int evtx_chnk_header_init(evtx_chnk_header_t **chk_header, const unsigned char *bytes) {
    evtx_chnk_header_t *ret;
    int pos = 0x0200;
    int i = 0;

    if (_check_first_eight(bytes) != 0) {
        /* TODO: Log failure */
        return NULL;
    }

    CALLOC(ret, 1, sizeof(evtx_chnk_header_t), return NULL);

    ret->first_event_record_num = eight_bytes_to_int64(bytes + 0x08);
    ret->last_event_record_num = eight_bytes_to_int64(bytes + 0x10);
    ret->first_event_record_id = eight_bytes_to_int64(bytes + 0x18);
    ret->last_event_record_id = eight_bytes_to_int64(bytes + 0x20);

    if (four_bytes_to_int32(bytes + 0x28) != 0x80) {
        /* TODO: Display warning */
    }

    ret->last_event_record_offset = four_bytes_to_int32(bytes + 0x2C);
    ret->free_space_offset = four_bytes_to_int32(bytes + 0x30);
    ret->event_record_data_crc32 = four_bytes_to_int32(bytes + 0x34);
    ret->flags = four_bytes_to_int32(bytes + 0x78);
    ret->header_crc32 = four_bytes_to_int32(bytes + 0x7C);

    CALLOC(ret->records, (size_t)(ret->last_event_record_num - ret->first_event_record_num),
        sizeof(evtx_record_t*), return 0);

    while (_check_next(bytes + pos) != 0) {
        pos += evtx_record_init(ret->records + i, bytes, pos);
    }

    *chk_header = ret;
    return pos;

    /* TODO: Check crc32 values */
    /* TODO: COMMON STRING OFFSET ARRAY */
    /* TODO: TEMPLATE PTR */
}