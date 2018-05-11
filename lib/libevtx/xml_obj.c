//
// Created by markzz on 5/11/18.
//

#include "xml_obj.h"

struct _evtx_xml_attr_t {
    char *name;
    char *value;
};

struct _evtx_xml_obj_t {
    char *name;

    int num_attrs;
    evtx_xml_attr_t **attrs;

    int num_children;
    struct _evtx_xml_obj_t **children;
};

