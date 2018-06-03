/*
 * util.c
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "util.h"

void _evtx_alloc_fail(size_t size) {
    fprintf(stderr, "allocation failure: could not allocate %zu bytes\n", size);
}

uint16_t two_bytes_to_int16(const unsigned char *bytes) {
    return (uint16_t)bytes[0] | (uint16_t)bytes[1] << 8;
}

uint32_t four_bytes_to_int32(const unsigned char *bytes) {
    return (uint32_t)two_bytes_to_int16(bytes) | (uint32_t)bytes[2] << 16 | (uint32_t)bytes[3] << 24;
}

uint64_t eight_bytes_to_int64(const unsigned char *bytes) {
    return (uint64_t) four_bytes_to_int32(bytes) | (uint64_t)bytes[4] << 32 | (uint64_t)bytes[5] << 40 |
            (uint64_t)bytes[6] << 48 | (uint64_t) bytes[7] << 56;
}

uint16_t _evtx_ms_str_hash(const char16_t *utf16_str, int len) {
    unsigned int val = 0;

    for (int i = 0; i < len; i++) {
        val = val * 65599 + utf16_str[i];
    }

    return (uint16_t)(val & 0xff);
}

time_t _filetime_to_unix_time(uint64_t filetime) {
    return (time_t)((filetime / 10000000) - 11644473600);
}

int _hash_match(uint16_t hash, const char16_t *utf16_str, int len) {
    return hash == _evtx_ms_str_hash(utf16_str, len);
}

