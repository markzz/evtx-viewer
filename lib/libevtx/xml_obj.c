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
#include <string.h>

#include "evtx_xml.h"
#include "util.h"

struct _evtx_xml_attr_t {
    char16_t *name;
    char16_t *value;
};

static char16_t *_evtx_read_name(const unsigned char *bytes, size_t *size) {
    /* first four bytes are currently unknown; perhaps log in debug? */
    uint16_t hash;
    size_t name_size;
    char16_t *name;

    hash = two_bytes_to_int16(bytes + 0x04);
    name_size = two_bytes_to_int16(bytes + 0x06);

    CALLOC(name, name_size + 1, sizeof(char16_t), return NULL);

    for (int i = 0; i < name_size; i++) {
        name[i] = two_bytes_to_int16(bytes + 0x08 + 2 * i);
    }

    if (_hash_match(hash, name, name_size) != 0) {
        /* TODO: Log warning */
    }

    if (size != NULL) {
        *size = name_size * 2 + 2;
    }
    return name;
}

int _parse_xml_obj(evtx_xml_obj_t **obj, const unsigned char *chnk_header, int offset, int in_template) {
    int pos = 1;
    int open;
    int data_size;
    size_t attr_size = 0;
    uint32_t name_offset;
    size_t tmp;
    char16_t *name;
    evtx_xml_obj_t *obj_p = *obj;

    if (chnk_header[0] != 0x01 && chnk_header[offset] != 0x41) {
        /* TODO: Log error */
        return 0;
    }
    open = (int)chnk_header[offset];

    if (in_template) {
        obj_p->dep = two_bytes_to_int16(chnk_header + offset + pos);
        pos += 2;
    }

    data_size = four_bytes_to_int32(chnk_header + offset + pos);
    pos += 4;

    name_offset = four_bytes_to_int32(chnk_header + offset + pos);
    pos += 4;

    if (name_offset == offset + pos) {
        name = _evtx_read_name(chnk_header + name_offset, &tmp);
        pos += tmp;
    } else {
        name = _evtx_read_name(chnk_header + name_offset, NULL);
    }
    obj_p->name = name;

    obj_p->num_attrs = 0;
    if (open & 0x40) {
        while (chnk_header[offset + pos] & 0x06) {
            obj_p->num_attrs += 1;
            obj_p->attrs = _evtx_greedy_grow((void**)obj_p->attrs, &attr_size, obj_p->num_attrs * sizeof(evtx_xml_attr_t));

            pos += 1;
            uint32_t attr_name_offset = four_bytes_to_int32(chnk_header + offset + pos);
            pos += 4;

            if (attr_name_offset == offset + pos) {
                name = _evtx_read_name(chnk_header + attr_name_offset, &tmp);
                pos += tmp;
            } else {
                name = _evtx_read_name(chnk_header + attr_name_offset, NULL);
            }

            CALLOC(obj_p->attrs[obj_p->num_attrs-1], 1, sizeof(evtx_xml_attr_t), return 0);

            obj_p->attrs[obj_p->num_attrs-1]->name = name;

            if (chnk_header[offset + pos] == 0x0d || chnk_header[offset + pos] == 0x0e) {
                /* we copy the next four bytes as if they are a string */
                CALLOC(obj_p->attrs[obj_p->num_attrs-1]->value, 4, sizeof(char), return 0);
                memcpy(obj_p->attrs[obj_p->num_attrs-1]->value, chnk_header + offset + pos, 4);
                pos += 4;
            } else if (chnk_header[offset + pos] == 0x05 && chnk_header[offset + pos + 1] == 0x01) {
                pos += 2;
                tmp = two_bytes_to_int16(chnk_header + offset + pos);
                pos += 2;
            }
        }
    }
}
