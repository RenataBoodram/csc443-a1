#include <stdio.h>

typedef struct record {
    int uid1;
    int uid2;
} Record;

Record *line_to_record(char *line);
