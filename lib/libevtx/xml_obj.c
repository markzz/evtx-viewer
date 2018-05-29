/*
 * xml_obj.c
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

#include <inttypes.h>

#include "evtx_xml.h"
#include "util.h"

struct _evtx_xml_attr_t {
    char *name;
    char *value;
};

int _parse_xml_obj(evtx_xml_obj_t **obj, const char *bytes, int in_template) {
    int pos = 1;
    int open;
    int data_size;
    int name_offset;
    evtx_xml_obj_t *obj_p = *obj;

    if (bytes[0] != 0x01 && bytes[0] != 0x41) {
        /* TODO: Log error */
        return 0;
    }
    open = (int)bytes[0];

    if (in_template) {
        obj_p->dep = two_bytes_to_int16(bytes + pos);
        pos += 2;
    }

    data_size = four_bytes_to_int32(bytes + pos);
    pos += 4;


}
