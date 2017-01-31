#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int block_size = 0;
 
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
    
    int total_follows = 0;
    int unique_users = 0;

    int max_user_follow_count = 0;
    int cur_user_follow_count = 0;
    //need this to identify unique users
    int cur_user_id = 0;

    float avg_num_followed = 0;
  
    // Calculate size of the binary file
    int file_size = find_file_size(file);

    // Total number of records in file
    int total_recs = file_size / rec_size;

    // Allocate a buffer
    Record *buffer = (Record *) calloc(total_recs, rec_size);
    // Load the entire file into the "RAM" buffer
    handle_fread_fwrite((rec_size * total_recs), "fread", buffer, 1,
        (rec_size * total_recs), file);

        //int i = 0;
        //int num_recs = 0;
        //while ((num_recs = fread(buffer, 1, block_size, file)) != 0)
        //{
    int i = 0;
    struct timeb t_begin, t_end;
    ftime(&t_begin);
    get_max_avg(buffer, total_recs, unique_users, total_follows, 
        max_user_follow_count);
    ftime(&t_end);
          //  printf("NUM_OF_RECS: %d\n", (num_recs / rec_size));
	    // get_max_avg(buffer, total_recs, unique_users, total_follows, max_user_follow_count);
	    /*
    while (i < total_recs)
    {
        printf("Record uid1: %d, Record uid2: %d\n", buffer[i].uid1, buffer[i].uid2); 
              
                //if new user, then increment unqiue users 
                //and reset follower counter back to 1
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

       // }


    avg_num_followed = (float)total_follows/unique_users;
    free(buffer);
    fclose(file);

    printf("Parsed %d, lines of data. \n", total_follows);
    printf("The most people followed is: %d. \n", max_user_follow_count);
    printf("Average number of follows is: %.2f. \n", avg_num_followed);
    printf("Max: %d\n", max_user_follow_count);
    printf("Average: %.2f\n" avg_num_followed);
    long time_spent_ms;
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
        + (t_end.millitm - t_begin.millitm));
    printf ("Data rate: %.3f MBPS\n",
        ((total_recs * rec_size)/(float)time_spent_ms * 1000)/(1024*1024));

    return 0;
}
