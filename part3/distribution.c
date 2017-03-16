#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "merge.h"

/*
 * Program:        distribution 
 * Description:    Read from a binary file in block size chunks. 
 * Authors:        boodram8, phamtyle
 */

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: distribution <file name> <block_size> " 
        "<column id> <max_degree>\n";
    char *input_file;
    int block_size = 0;
    int records_per_block = 0;
    char *col_id = 0;
    int max_deg = 0;
 
    if (argc != 5)
    {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        block_size = (int) strtol(argv[2], (char **)NULL, 10);
        col_id = argv[3];
        max_deg = (int) strtol(argv[4], (char **)NULL, 10);

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

    // Create an array to hold the results (+ 1 to include the 0 option)
    int counts[max_deg + 1];
    int i = 0;
    while (i < (max_deg + 1))
    {
        counts[i] = 0;
        i = i + 1;
    }
    int num_recs = 0;
    int cur_user_id = -1;           // Keep track of current user ID   
    int cur_user_count = 0;            // Index for counts array

    Record *buffer = (Record *) calloc(records_per_block, rec_size);

    
    while ((num_recs = fread(buffer, 1, block_size, file)) != 0)
    {
         get_deg_count(buffer, (num_recs/rec_size), &cur_user_id, &cur_user_count, col_id, counts); 
        
    }
    counts[cur_user_count] = counts[cur_user_count] + 1;

    fclose(file);
    free(buffer);

    // Print out the values
    int counts_ind = 0;
    while (counts_ind < max_deg + 1)
    {
        printf("(%d, %d)\n", counts_ind, counts[counts_ind]);
        counts_ind = counts_ind + 1;
    }

    return 0;
}
