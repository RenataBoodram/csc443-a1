#include <stdio.h>

typedef struct record {
    int uid1;
    int uid2;
} Record;

enum {
    rec_size = sizeof(Record),
};

Record *line_to_record(char *str_arr[]);
void parse_line(char *line, char *str_arr[]);
void handle_fread_fwrite(int bytes, const char *func, void *ptr, size_t size,
    size_t nmemb, FILE *stream);
void handle_fseek(FILE *stream, long offset, int whence);
