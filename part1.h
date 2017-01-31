#include <stdio.h>

typedef struct record {
    int uid1;
    int uid2;
} Record;

enum {
    rec_size = sizeof(Record),
};

/* Returns array of strings given a line in a CSV file. */
void parse_line(char *line, char *str_arr[]);

/* Returns Record given array of Record fields. */
Record *line_to_record(char *str_arr[]);

/* Wrapper function to handle fread and fwrite errors. */
void handle_fread_fwrite(int bytes, const char *func, void *ptr, size_t size,
    size_t nmemb, FILE *stream);

/* Wrapper function to handle fseek errors. */
void handle_fseek(FILE *stream, long offset, int whence);

/* Returns size of file. Terminates on error. */
int find_file_size(FILE *file);

/*
 * Update max value and other variables required to calculate the avg where        
 * avg = total_follows/unique_users.  
 */  
void get_max_avg(Record *buffer, int cond, int *max, int *unique_users, int *total_follows, int *cur_user_id, int *cur_user_follow_count);

/* Prints total data rate time. Prints avg and max values if non-zero. */ 
void print_vals(struct timeb t_begin, struct timeb t_end, int total_bytes, int max, float avg); 
