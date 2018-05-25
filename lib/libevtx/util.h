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

#include "evtx.h"

void _evtx_alloc_fail(size_t size);

#define MALLOC(p, s, action) do { p = malloc(s); if (p == NULL) { _evtx_alloc_fail(s); action; } } while(0)
#define CALLOC(p, l, s, action) do { p = calloc(l, s); if(p == NULL) { _evtx_alloc_fail(l * s); action; } } while(0)

#define FREE(p) do { free(p); p = NULL; } while(0)

#define ASSERT(cond, action) do { if(!(cond)) { action; } } while(0)

int _evtx_log(evtx_log_level_t level, const char *format, ...);

int two_bytes_to_int(const char *bytes);
int four_bytes_to_int(const char *bytes);
long eight_bytes_to_long(const char *bytes);

unsigned long _filetime_to_unix_time(long long filetime);
int _hash_match(uint16_t hash, const char *utf16_str, int len);

#endif //_UTIL_H
