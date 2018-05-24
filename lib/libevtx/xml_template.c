/*
 * xml_template.c
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

#include "evtx_xml.h"
#include "util.h"

int _parse_template(evtx_xml_obj_t **obj, const char *bytes) {
    int pos = 1;
    int size = 0;
    int data_size = 0;

    if (bytes[0] != 0x0C) {
        /* TODO: Log error */
        return 0;
    }

    /* TODO: debug log next irrelevant byte (should be 0x01) */
    /* TODO: debug log template id */

    size = four_bytes_to_int(bytes + 0x06);

    /* TODO: debug log next four bytes (next template def offset) */
    /* TODO: debug log template GUID */

    data_size = four_bytes_to_int(bytes + 0x1E);

    pos += parse_fragment(&obj, bytes + 0x22);

    return data_size;
}