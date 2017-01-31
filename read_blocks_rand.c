#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>
#include "part1.h"

/*
 * Program:        read_blocks_rand 
 * Description:    Read from a binary file in block size chunks. 
 * Authors:        boodram8, phamtyle
 */


int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: write_blocks_rand <input filename> "
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

	if(block_size % rec_size != 0)
	{
	printf("Please enter a valid blocksize");
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
    handle_fseek(file, 0L, SEEK_END); 
    int file_size = ftell(file);
    if (file_size == -1)
    {
        printf("Error when determining file size. Exiting.\n");
        fclose(file);
        exit(1);
    }

    rewind(file);

    int num_of_recs = file_size/rec_size;

    int records_per_block = block_size/rec_size;

    int total_follows = 0;
    int unique_users = 0;

    int max_user_follow_count = 0;
    int cur_user_follow_count = 0;
    //need this to identify unique users
    int cur_user_id = 0;

    float avg_num_followed = 0;
    
    srand(time(NULL)); // Initialize random seed
    int i = 0;
    long rec_num = 0;
    struct timeb t_begin, t_end;
 
    Record *buffer = (Record *) calloc(records_per_block, rec_size);

    ftime(&t_begin);

    while (i < num_rand_reads)
    {
        // Choose a random record number in interval [0, num_of_recs - 1].
        rec_num = rand() % (num_of_recs - records_per_block); 
    
        printf("Record %ld chosen\n", rec_num);
        // Seek to the start of the record
        handle_fseek(file, (rec_num * rec_size), SEEK_SET);
         
        handle_fread_fwrite(block_size,"fread", buffer, 1, block_size, file);

       	// Read the block
           int j = 0;
            printf("RECORDS PER BLOCK: %d\n", records_per_block);
            while (j < records_per_block)
            {
                printf("Record uid1: %d, Record uid2: %d\n", buffer[j].uid1, buffer[j].uid2);

                //if new user, then increment unqiue users 
                //and reset follower counter back to 1
                if(cur_user_id != buffer[j].uid1)
                {
                    cur_user_id = buffer[j].uid1;
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
                j++;
            }


        i++;
    }    ftime(&t_end);
    //free(rec);
    fclose(file);
    long time_spent_ms;
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
        + (t_end.millitm - t_begin.millitm));
    printf ("Data rate: %.3f BPS\n", 
        ((num_rand_reads*rec_size)/(float)time_spent_ms * 1000));

    avg_num_followed = (float)total_follows/unique_users;
    //fclose(file);
    printf("Parsed %d, lines of data. \n", total_follows);
    printf("The most people followed is: %d. \n", max_user_follow_count);
    printf("Average number of follows is: %.2f. \n", avg_num_followed);

    return 0;
}   
