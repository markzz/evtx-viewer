#include <stdio.h>
#include <stdlib.h>

#include "../lib/libevtx/evtx.h"

int main() {
    printf("Hello, World!\n");

    FILE *fp = fopen("/home/markzz/Application.evtx", "rb");
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);

    char *buf = (char *)malloc(len * sizeof(char));
    fread(buf, len, 1, fp);

    evtx_t *a = evtx_header_init(buf);

    return 0;
}