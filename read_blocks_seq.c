#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "part1.h"

/*
 * Program:        read_blocks_seq 
 * Description:    Read from a binary file in block size chunks. 
 * Authors:        boodram8, phamtyle
 */

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: read_blocks_seq <input filename> "
        "<block_size>\n";
    char *input_file;
    int block_size = 0;
    int records_per_block = 0;
 
    if (argc != 3) 
    {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        block_size = (int) strtol(argv[2], (char **)NULL, 10);

        if(block_size % rec_size != 0)
	{
            printf("%d is not a valid block size. Must be a multiple of 8.\n", 
	        block_size);
            exit(1);
        } else 
	{
            records_per_block = block_size / rec_size;
        }
    }

    FILE *file = fopen(input_file, "rb");
    if (!file)
    {
        printf("Could not open file for reading. Exiting.\n");
	exit(1);
    }

    int total_follows = 0;         // Total number of follow relationships 
    int unique_users = 0;          // Total number of unique users 
    int cur_user_follow_count = 0; // Follow count for current user                   
    int cur_user_id = 0;           // Keep track of current user ID   
    float avg_num_followed = 0; 

    Record *buffer = (Record *) calloc(records_per_block, rec_size);
    int num_recs = 0;
    struct timeb t_begin, t_end;

    long total_bytes = 0;

    int max = 0;

    ftime(&t_begin);
    while ((num_recs = fread(buffer, 1, block_size, file)) != 0)
    {
        total_bytes = total_bytes + num_recs;
	get_max_avg(buffer, (num_recs/rec_size), &max, &unique_users, 
            &total_follows, &cur_user_id, &cur_user_follow_count); 

    }

    avg_num_followed = (float) total_follows/unique_users;
    fclose(file);
    ftime(&t_end);
    free(buffer);
    
    print_vals(t_begin, t_end, total_bytes, max, avg_num_followed);
    return 0;
}
