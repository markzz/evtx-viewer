/*
 * evtx_xml.h
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

#ifndef EVTX_VIEWER_XML_OBJ_H
#define EVTX_VIEWER_XML_OBJ_H

#include <inttypes.h>

typedef struct _evtx_xml_attr_t evtx_xml_attr_t;
typedef struct _evtx_xml_obj_t evtx_xml_obj_t;
typedef struct _evtx_xml_template_t evtx_xml_template_t;

struct _evtx_xml_obj_t {
    char *name;
    int16_t dep;

    int num_attrs;
    evtx_xml_attr_t **attrs;

    int num_children;
    struct _evtx_xml_obj_t **children;
};

int parse_fragment(evtx_xml_obj_t **obj_p, const char *bytes, int in_template);

int _parse_template(evtx_xml_obj_t **obj, const char *bytes);

int _parse_xml_obj(evtx_xml_obj_t **obj, const char *bytes, int in_template);


#endif //EVTX_VIEWER_XML_OBJ_H
