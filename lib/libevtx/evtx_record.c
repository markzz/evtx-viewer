/*
 * evtx_record.c
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

#include <time.h>

#include "evtx_record.h"
#include "evtx_xml.h"
#include "util.h"

struct _evtx_record_t {
    long event_record_id;
    time_t record_time;

    evtx_xml_obj_t *xml_obj;
};

static int _check_magic(const char *bytes) {
    char expected[] = {0x2A, 0x2A, 0x00, 0x00};
    for (int i = 0; i < 4; i++) {
        if (bytes[i] != expected[i]) {
            return -1;
        }
    }
    return 0;
}

int evtx_record_init(evtx_record_t **record, const char *bytes) {
    evtx_record_t *ret;
    int size = 0;
    long long tmp_ft;

    if (_check_magic(bytes) != 0) {
        /* TODO: Log failure */
        return NULL;
    }

    CALLOC(ret, 1, sizeof(evtx_record_t), return NULL);

    /* TODO: Log size (debugging output) */
    size = four_bytes_to_int(bytes + 0x04);

    ret->event_record_id = eight_bytes_to_long(bytes + 0x08);

    tmp_ft = eight_bytes_to_long(bytes + 0x10);
    ret->record_time = _filetime_to_unix_time(tmp_ft);

    size = 0x18 + parse_fragment(&ret->xml_obj, bytes + 0x18);

    *record = ret;
    return size;
}