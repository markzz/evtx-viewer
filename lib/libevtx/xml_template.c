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
#include <string.h>
#include <inttypes.h>

#include "evtx_xml.h"
#include "util.h"

int _parse_template(evtx_xml_obj_t **obj, const unsigned char *chnk_header, int offset) {
    int pos = 0x22;
    int size = 0;
    int data_size = 0;
    uint32_t vspec_num_items;
    int **vspec = NULL;
    void **values;

    if (chnk_header[offset] != 0x0C) {
        /* TODO: Log error */
        return 0;
    }

    /* TODO: debug log next irrelevant byte (should be 0x01) */
    /* TODO: debug log template id */

    size = four_bytes_to_int32(chnk_header + offset + 0x06);

    /* TODO: debug log next four bytes (next template def offset) */
    /* TODO: debug log template GUID */

    data_size = four_bytes_to_int32(chnk_header + offset + 0x1E);

    pos += parse_fragment(obj, chnk_header, offset + 0x22, 1);

    vspec_num_items = four_bytes_to_int32(chnk_header + offset + pos);
    pos += 4;
    CALLOC(vspec, vspec_num_items, sizeof(int*), return 0);
    CALLOC(values, vspec_num_items, sizeof(void*), return 0);

    for (int i = 0; i < vspec_num_items; i++) {
        CALLOC(vspec[i], 2, sizeof(int), return 0);

        vspec[i][0] = two_bytes_to_int16(chnk_header + offset + pos);
        vspec[i][1] = two_bytes_to_int16(chnk_header + offset + pos + 2);
        pos += 4;
    }

    /* the fun part, this crap... */
    for (int i = 0; i < vspec_num_items; i++) {
        switch (vspec[i][1]) {
            case EVTX_TYPE_STRING:
            case EVTX_TYPE_ASCII_STRING:
            case EVTX_TYPE_BINARY: /* I'm not entirely sure what this is, so we are handling it here */
            case EVTX_TYPE_GUID:
                CALLOC(values[i], (size_t)vspec[i][0], sizeof(char), return 0);
                memcpy(values[i], chnk_header + offset + pos, (size_t)vspec[i][0]);
                pos += vspec[i][0];
                break;
            case EVTX_TYPE_INT8:
            case EVTX_TYPE_UINT8:
            case EVTX_TYPE_BOOL:
                CALLOC(values[i], 1, sizeof(int8_t), return 0);
                *((int8_t*)values[i]) = *(chnk_header + offset + pos);
                pos++;
                break;
            case EVTX_TYPE_INT16:
            case EVTX_TYPE_UINT16:
                CALLOC(values[i], 1, sizeof(int16_t), return 0);
                *((int16_t*)values[i]) = (int16_t) two_bytes_to_int16(chnk_header + offset + pos);
                pos += 2;
                break;
            case EVTX_TYPE_INT32:
            case EVTX_TYPE_UINT32:
            case EVTX_TYPE_HEX_INT32:
            case EVTX_TYPE_REAL32: /* this one does MAGIC */
                CALLOC(values[i], 1, sizeof(int32_t), return 0);
                *((int32_t*)values[i]) = (int32_t) four_bytes_to_int32(chnk_header + offset + pos);
                pos += 4;
                break;
            case EVTX_TYPE_INT64:
            case EVTX_TYPE_UINT64:
            case EVTX_TYPE_HEX_INT64:
            case EVTX_TYPE_REAL64: /* this one does MAGIC */
                CALLOC(values[i], 1, sizeof(int64_t), return 0);
                *((int64_t*)values[i]) = (int64_t) eight_bytes_to_int64(chnk_header + offset + pos);
                pos += 8;
                break;
            case EVTX_TYPE_BIN_XML:
                CALLOC(values[i], 1, sizeof(evtx_xml_obj_t), return 0);
                parse_fragment((evtx_xml_obj_t **) &values[i], chnk_header + offset + pos, 0, 1);
                pos += vspec[i][0];
                break;
            case EVTX_TYPE_FILE_TIME:
                CALLOC(values[i], 1, sizeof(time_t), return 0);
                *((time_t*)values[i]) = _filetime_to_unix_time(eight_bytes_to_int64(chnk_header + offset + pos));
                pos += 8;
                break;
            case EVTX_TYPE_SIZE_T:
            case EVTX_TYPE_SYS_TIME:
            case EVTX_TYPE_SID:
            case EVTX_TYPE_HANDLE:
            case EVTX_TYPE_EVT_XML:
            case EVTX_TYPE_ARRAY_USTRING:
            case EVTX_TYPE_ARRAY_STRING:
            case EVTX_TYPE_ARRAY_INT8:
            case EVTX_TYPE_ARRAY_UINT8:
            case EVTX_TYPE_ARRAY_INT16:
            case EVTX_TYPE_ARRAY_UINT16:
            case EVTX_TYPE_ARRAY_INT32:
            case EVTX_TYPE_ARRAY_UINT32:
            case EVTX_TYPE_ARRAY_INT64:
            case EVTX_TYPE_ARRAY_UINT64:
            case EVTX_TYPE_ARRAY_REAL32:
            case EVTX_TYPE_ARRAY_REAL64:
            case EVTX_TYPE_ARRAY_BOOL:
            case EVTX_TYPE_ARRAY_GUID:
            case EVTX_TYPE_ARRAY_SIZE_T:
            case EVTX_TYPE_ARRAY_FILE_TIME:
            case EVTX_TYPE_ARRAY_SYS_TIME:
            case EVTX_TYPE_ARRAY_SID:
            case EVTX_TYPE_ARRAY_HEX32:
            case EVTX_TYPE_ARRAY_HEX64:
            case EVTX_TYPE_NULL:
            default:
                break;
        }
    }

    return size;
}