#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>
#include "part1.h"

/*
 * Program:        read_blocks_rand 
 * Description:    Read from a binary file in block size chunks at X random
 *                 positions. 
 * Authors:        boodram8, phamtyle
 */
int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: read_blocks_rand <input filename> "
        "<block size> <X number of random reads>\n";
    char *input_file; // Input filename
    int num_rand_reads = 0;
    int block_size = 0;
 
    if (argc != 4) {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        block_size = (int) strtol(argv[2], (char **)NULL, 10);

	if (block_size % rec_size != 0)
	{
	    printf("%d is not a valid block size. Must be a multiple of 8.\n",
                block_size);
	    exit(1);
	}

        num_rand_reads = (int) strtol(argv[3], (char **)NULL, 10);
    }

    FILE *file = fopen(input_file, "rb");

    if (!file) 
    {
        printf("Input file could not be opened for reading. Exiting.\n");
        exit(1);
    }

    // Calculate size of the binary file
    int file_size = 0;
    file_size = find_file_size(file);

    int num_of_recs = file_size / rec_size;

    int records_per_block = block_size / rec_size;

    int total_follows = 0;         // Total number of follow relationships 
    int unique_users = 0;          // Total number of unique users                  
    int cur_user_follow_count = 0; // Follow count for current user                
    int cur_user_id = 0;           // Keep track of current user ID    

    int max = 0;

    float avg_num_followed = 0;
    
    srand(time(NULL)); // Initialize random seed
    int i = 0;
    long rec_num = 0;
    struct timeb t_begin, t_end;
 
    Record *buffer = (Record *) calloc(records_per_block, rec_size);

    ftime(&t_begin);

    while (i < num_rand_reads)
    {
        /* Choose a random record number in interval 
         * [0, num_of_recs - records_per_block - 1].
         */
        rec_num = rand() % (num_of_recs - records_per_block); 
    
        // Seek to the start of the record
        handle_fseek(file, (rec_num * rec_size), SEEK_SET);
         
        handle_fread_fwrite(block_size,"fread", buffer, 1, block_size, file);

        get_max_avg(buffer, records_per_block, &max, &unique_users, 
            &total_follows, &cur_user_id, &cur_user_follow_count);

        i++;
    } 
    avg_num_followed = (float) total_follows / unique_users;
    fclose(file);
    ftime(&t_end);
    free(buffer);

    print_vals(t_begin, t_end, (num_rand_reads * records_per_block * rec_size),
        max, avg_num_followed);

    return 0;
}   
