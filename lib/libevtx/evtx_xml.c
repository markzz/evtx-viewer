/*
 * evtx_xml.c
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

#include "evtx_record.h"
#include "evtx_xml.h"
#include "util.h"

static int _check_magic(const unsigned char *bytes) {
    char expected[] = {0x0F, 0x01, 0x01, 0x00};
    for (int i = 0; i < 4; i++) {
        if (bytes[i] != expected[i]) {
            return -1;
        }
    }
    return 0;
}

int parse_fragment(evtx_xml_obj_t **obj_p, const unsigned char *chnk_header, int offset, int in_template) {
    evtx_xml_obj_t *ret = NULL;
    int pos = 4;

    if (_check_magic(chnk_header + offset) != 0) {
        /* TODO: Log failure */
        return 0;
    }

    CALLOC(ret, 1, sizeof(evtx_xml_obj_t), return 0);

    if (chnk_header[offset + pos] == 0x0C) {
        pos += _parse_template(&ret, chnk_header, offset + pos);
    } else if (chnk_header[offset + pos] == 0x01 || chnk_header[offset + pos] == 0x41) {
        pos += _parse_xml_obj(&ret, chnk_header, offset + pos, in_template);
    } else {
        /* TODO: log error */
        return 0;
    }

    *obj_p = ret;
    return pos;
}