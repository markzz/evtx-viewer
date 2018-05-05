//
// Created by markzz on 5/4/18.
//

#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>

void _evtx_alloc_fail(size_t size);

#define MALLOC(p, s, action) do { p = malloc(s); if (p == NULL) { _evtx_alloc_fail(s); action; } } while(0)
#define CALLOC(p, l, s, action) do { p = calloc(l, s); if(p == NULL) { _evtx_alloc_fail(l * s); action; } } while(0)

#define FREE(p) do { free(p); p = NULL; } while(0)

#define ASSERT(cond, action) do { if(!(cond)) { action; } } while(0)

int two_bytes_to_int(const char *bytes);
int four_bytes_to_int(const char *bytes);
long eight_byes_to_long(const char *bytes);

#endif //_UTIL_H
