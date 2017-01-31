// 2.3 Sequential Reads

// TODO
// 1.read binary record file in blocks
// 2. return the user who is following the most amount of people [max]
// 3. return the average of each users following

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "read_ram_seq.h"

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: read_ram_seq <filename><block_size>\n";
    char *input_file;
    int block_size = 0;
    int records_per_block = 0;
 
    if (argc != 3) {
        printf("%s", usage_msg);
        exit(1);
    } 
    else {
        input_file = argv[1];
        block_size = (int) strtol(argv[2], (char **)NULL, 10);

        if(block_size % rec_size != 0){
            printf("%s.\n", "Not a valid block size");
            exit(1);
        }
        else{
        records_per_block = block_size / rec_size;
        printf("RECORDS_PER_BLOCK %d\n", (int)records_per_block);
        }
    }

    FILE *file = fopen(input_file, "r");
    ssize_t read;
    char *line = NULL;
    size_t n = 0;
    
    int total_follows = 0;
    int unique_users = 0;

    int max_user_follow_count = 0;
    int cur_user_follow_count = 0;
    //need this to identify unique users
    int cur_user_id = 0;

    float avg_num_followed = 0; 

    Record *buffer = (Record *) calloc(records_per_block, rec_size);
    if (file){
        //int i = 0;
        int num_recs = 0;
        while ((num_recs = fread(buffer, 1, block_size, file)) != 0)
        {
            int i = 0;
            //printf("NUM_OF_RECS: %d\n", (num_recs / rec_size));
            while (i < (num_recs/rec_size))
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


        }

    } else {
        printf("File %s could not be found. Exiting.\n", input_file);
        exit(1);
    }

    avg_num_followed = (float)total_follows/unique_users;
    fclose(file);

    printf("Parsed %d, lines of data. \n", total_follows);
    printf("The most people followed is: %d. \n", max_user_follow_count);
    printf("Average number of follows is: %.2f. \n", avg_num_followed);
    return 0;
}
