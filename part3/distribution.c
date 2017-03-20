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

int distribution(char *input_file, char *output_file, int degree) 
{
    int block_size = blk_size;
    int total_mem_blocks = entire_mem / blk_size;
    int records_per_block = block_size / rec_size;
    int num_output_recs = (total_mem_blocks - 1)*records_per_block;
    FILE *file = fopen(input_file, "rb");
    if (!file)
    {
        printf("Could not open file for reading. Exiting.\n");
	exit(1);
    }

    FILE *output;


    // Create an array to hold the results (+ 1 to include the 0 option)
    int num_recs = 0;
    int cur_user_id = -1;           // Keep track of current user ID   
    int curr_id = -1;

    Record *buffer = (Record *) calloc(records_per_block, rec_size);

    // Create an output buffer so we can write multiple records at once 
    Record2 *output_buffer = (Record2 *) calloc(num_output_recs, rec_size);

    int i = 0;
    int count = 1; 
    int tot = 0;
    int j = 0;
    while ((num_recs = fread(buffer, 1, block_size, file)) != 0)
    {
        i = 0;
        while (i < (num_recs/rec_size)) { 
            if (degree == 0) { // if degree == OUT 
                curr_id = buffer[i].UID1;
            } else { // degree == IN 
                curr_id = buffer[i].UID2;
            }
            // If the current user id has never been set, set it
            if (cur_user_id == -1) {
                cur_user_id = curr_id;
            // Check if cur_user_id is the same as the current ID
            } else if (cur_user_id != curr_id)
            {
                output_buffer[j].UID = cur_user_id;
                output_buffer[j].deg = count;
                tot++;
                j++;
                if (tot >= num_output_recs) {
                output = fopen(output_file, "a");
                if (!output)
                {
                    printf("Could not open file for writing. Exiting.\n");
                    exit(1);
                }

                //Record2 *temp_rec = malloc(rec2_size);
                //temp_rec->UID = cur_user_id;
                //temp_rec->deg = count;
                // Reset the counts_ind for the next cur_user_id
                //int result = fwrite(temp_rec, rec2_size, 1, output);
                int result = fwrite(output_buffer, rec2_size, num_output_recs, output);
                if (result < 0) {
                    perror("Error when writing\n");
                }
                //free(temp_rec);
                fclose(output);
                tot = 0;
                }
                cur_user_id = curr_id;
                count = 1;
            } else { //cur_user_id == curr_id
                count++;
            }
            /* Keep incrementing the counter in the counts array. The value of
             * counts[counts_ind] won't change until we change it.
             */
            //count++;

            i++;
        }
        
        
    }

// Write remaining elements

                output = fopen(output_file, "a");
                if (!output)
                {
                    printf("Could not open file for writing. Exiting.\n");
                    exit(1);
                }

                int result = fwrite(output_buffer, rec2_size, num_output_recs, output);
                if (result < 0) {
                    perror("Error when writing\n");
                }
                //free(temp_rec);
                fclose(output);


    fclose(file);
    free(buffer);

    return 0;
}
