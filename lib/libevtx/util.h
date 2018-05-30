/*
 * util.h
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

#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>
#include <inttypes.h>

#include "evtx.h"

typedef enum _evtx_value_type_t {
    EVTX_TYPE_NULL,
    EVTX_TYPE_STRING,
    EVTX_TYPE_ASCII_STRING,
    EVTX_TYPE_INT8,
    EVTX_TYPE_UINT8,
    EVTX_TYPE_INT16,
    EVTX_TYPE_UINT16,
    EVTX_TYPE_INT32,
    EVTX_TYPE_UINT32,
    EVTX_TYPE_INT64,
    EVTX_TYPE_UINT64,
    EVTX_TYPE_REAL32,
    EVTX_TYPE_REAL64,
    EVTX_TYPE_BOOL,
    EVTX_TYPE_BINARY,
    EVTX_TYPE_GUID,
    EVTX_TYPE_SIZE_T,
    EVTX_TYPE_FILE_TIME,
    EVTX_TYPE_SYS_TIME,
    EVTX_TYPE_SID,
    EVTX_TYPE_HEX_INT32,
    EVTX_TYPE_HEX_INT64,

    EVTX_TYPE_HANDLE = 0x20,               /* unimplemented */
    EVTX_TYPE_BIN_XML,

    EVTX_TYPE_EVT_XML = 0x23,              /* unimplemented */

    EVTX_TYPE_ARRAY_USTRING = 0x81,
    EVTX_TYPE_ARRAY_STRING,
    EVTX_TYPE_ARRAY_INT8,
    EVTX_TYPE_ARRAY_UINT8,
    EVTX_TYPE_ARRAY_INT16,
    EVTX_TYPE_ARRAY_UINT16,
    EVTX_TYPE_ARRAY_INT32,
    EVTX_TYPE_ARRAY_UINT32,
    EVTX_TYPE_ARRAY_INT64,
    EVTX_TYPE_ARRAY_UINT64,
    EVTX_TYPE_ARRAY_REAL32,
    EVTX_TYPE_ARRAY_REAL64,
    EVTX_TYPE_ARRAY_BOOL,

    EVTX_TYPE_ARRAY_GUID = 0x8F,
    EVTX_TYPE_ARRAY_SIZE_T,
    EVTX_TYPE_ARRAY_FILE_TIME,
    EVTX_TYPE_ARRAY_SYS_TIME,
    EVTX_TYPE_ARRAY_SID,
    EVTX_TYPE_ARRAY_HEX32,
    EVTX_TYPE_ARRAY_HEX64,
} evtx_value_type_t;

void _evtx_alloc_fail(size_t size);

#define MALLOC(p, s, action) do { p = malloc(s); if (p == NULL) { _evtx_alloc_fail(s); action; } } while(0)
#define CALLOC(p, l, s, action) do { p = calloc(l, s); if(p == NULL) { _evtx_alloc_fail(l * s); action; } } while(0)

#define FREE(p) do { free(p); p = NULL; } while(0)

#define ASSERT(cond, action) do { if(!(cond)) { action; } } while(0)

int _evtx_log(evtx_log_level_t level, const char *format, ...);

uint16_t two_bytes_to_int16(const unsigned char *bytes);
uint32_t four_bytes_to_int32(const unsigned char *bytes);
uint64_t eight_bytes_to_int64(const unsigned char *bytes);

time_t _filetime_to_unix_time(uint64_t filetime);
int _hash_match(uint16_t hash, const char *utf16_str, int len);

char *_get_string_from_offset(const char *search, int offset);

#endif //_UTIL_H
