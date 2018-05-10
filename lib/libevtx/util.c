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

#include "util.h"

void _evtx_alloc_fail(size_t size) {
    fprintf(stderr, "allocation failure: could not allocate %zu bytes\n", size);
}

int two_bytes_to_int(const char *bytes) {
    return bytes[0] | bytes[1] << 8;
}

int four_bytes_to_int(const char *bytes) {
    return two_bytes_to_int(bytes) | bytes[2] << 16 | bytes[3] << 24;
}

long eight_bytes_to_long(const char *bytes) {
    return (long)four_bytes_to_int(bytes) | bytes[4] << 32 | bytes[5] << 40 |
           bytes[6] << 48 | bytes[7] << 56;
}