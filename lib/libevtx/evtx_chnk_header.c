//
// Created by markzz on 5/5/18.
//

#include "evtx_chnk_header.h"
#include "util.h"

struct _evtx_chnk_header_t {
    long first_event_record_num;
    long last_event_record_num;
    long first_event_record_id;
    long last_event_record_id;
    int last_event_record_offset;
    int free_space_offset;
    int event_record_data_crc32;
    int flags;
    int header_crc32;
};

static int _check_first_eight(const char *bytes) {
    char expected[] = {'E', 'l', 'f', 'C', 'h', 'n', 'k', 0};
    for (int i = 0; i < 8; i++) {
        if (bytes[i] != expected[i]) {
            return -1;
        }
    }
    return 0;
}

evtx_chnk_header_t *evtx_chnk_header_init(const char *bytes) {
    evtx_chnk_header_t *ret;

    if (_check_first_eight(bytes) != 0) {
        /* TODO: Log failure */
        return NULL;
    }

    CALLOC(ret, 1, sizeof(evtx_chnk_header_t), return NULL);

    ret->first_event_record_num = eight_byes_to_long(bytes + 0x08);
    ret->last_event_record_num = eight_byes_to_long(bytes + 0x10);
    ret->first_event_record_id = eight_byes_to_long(bytes + 0x18);
    ret->last_event_record_id = eight_byes_to_long(bytes + 0x20);

    if (four_bytes_to_int(bytes + 0x28) != 0x80) {
        /* TODO: Display warning */
    }

    ret->last_event_record_offset = four_bytes_to_int(bytes + 0x2C);
    ret->free_space_offset = four_bytes_to_int(bytes + 0x30);
    ret->event_record_data_crc32 = four_bytes_to_int(bytes + 0x34);
    ret->flags = four_bytes_to_int(bytes + 0x78);
    ret->header_crc32 = four_bytes_to_int(bytes + 0x7C);

    /* TODO: Check crc32 values */
    /* TODO: COMMON STRING OFFSET ARRAY */
    /* TODO: TEMPLATE PTR */
}