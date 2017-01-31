#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "part1.h"

/*
 * Program:        read_ram_seq 
 * Description:    Read from a binary file stored in a RAM buffer. 
 * Authors:        boodram8, phamtyle
 */

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: read_ram_seq <input filename>\n";
    char *input_file;
 
    if (argc != 2)
    {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
    }

    FILE *file = fopen(input_file, "rb");
    if (!file)
    {
        printf("Unable to open file for reading. Exiting.\n");
        exit(1); 
    }
    
    int total_follows = 0;         // Total number of follow relationships
    int unique_users = 0;          // Total number of unique users
    int cur_user_follow_count = 0; // Follow count for current user
    int cur_user_id = 0;           // Keep track of current user ID
    float avg_num_followed = 0;
  
    // Calculate size of the binary file
    int file_size = find_file_size(file);

    // Total number of records in file
    int total_recs = file_size / rec_size;

    // Allocate a buffer
    Record *buffer = (Record *) calloc(total_recs, rec_size);
    // Load the entire file into the "RAM" buffer
    handle_fread_fwrite(file_size, "fread", buffer, 1, file_size, file);

    int max = 0;
    struct timeb t_begin, t_end;
    ftime(&t_begin);
    get_max_avg(buffer, total_recs, &max, &unique_users, &total_follows, &cur_user_id, &cur_user_follow_count);
    fclose(file);
    ftime(&t_end);
    free(buffer);

    avg_num_followed = (float)total_follows/unique_users;
 
    print_vals(t_begin, t_end, file_size, max, avg_num_followed);

    return 0;
}
