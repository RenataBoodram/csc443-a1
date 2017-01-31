#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "part1.h"

/*
 * Program:        read_blocks_seq 
 * Description:    Read from a CSV file and write to a new CSV file.
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
        printf("Could not open file for reading. Exiting.\n")
	exit(1);
    }
    
    int total_follows = 0;   
    int unique_users = 0;

    int max_user_follow_count = 0;
    int cur_user_follow_count = 0;
    //need this to identify unique users
    int cur_user_id = 0;

    float avg_num_followed = 0; 

    Record *buffer = (Record *) calloc(records_per_block, rec_size);
    int num_recs = 0;
    struct timeb t_begin, t_end;

    long total_bytes = 0;

    ftime(&t_begin);
    while ((num_recs = fread(buffer, 1, block_size, file)) != 0)
    {
        total_bytes = total_bytes + num_recs;
        int i = 0;
	get_max_avg(buffer, (num_recs/rec_size), unique_users, total_follows, 
	    max_user_follow_count);
        //printf("NUM_OF_RECS: %d\n", (num_recs / rec_size));
/*        while (i < (num_recs/rec_size))
        {
            printf("Record uid1: %d, Record uid2: %d\n", buffer[i].uid1, buffer[i].uid2); 
              
            // If there is a new user, increment unique_users and reset
	    // reset counter back to 1 
            if(cur_user_id != buffer[i].uid1)
            {
                cur_user_id = buffer[i].uid1;
                unique_users++;
                cur_user_follow_count = 1;
            } else {
                cur_user_follow_count++;
            }

            if (cur_user_follow_count > max_user_follow_count) 
            {
                max_user_follow_count = cur_user_follow_count;
            }

            total_follows++;
            i++;
        } 
*/

    }

    avg_num_followed = (float)total_follows/unique_users;
    fclose(file);
    ftime(&t_end);
    free(buffer);

    //printf("Parsed %d, lines of data. \n", total_follows);
    //printf("The most people followed is: %d. \n", max_user_follow_count);
    //printf("Average number of follows is: %.2f. \n", avg_num_followed);
    printf("Max: %d\n", max_user_follow_count);
    printf("Average: %.2f\n" avg_num_followed);
    long time_spent_ms;
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time) 
        + (t_end.millitm - t_begin.millitm));
    printf ("Data rate: %.3f MBPS\n",
        (total_bytes/(float)time_spent_ms * 1000)/(1024*1024));

    return 0;
}
