#include <stdlib.h>
#include "merge.h"

/**
* Compares two records a and b 
* with respect to the value of the integer field f.
* Returns an integer which indicates relative order: 
* positive: record a > record b
* negative: record a < record b
* zero: equal records
* Credit: Marina Barsky for function
*/
int compare (const void *a, const void *b) 
{
   int a_f = ((const Record*)a)->UID2;
   int b_f = ((const Record*)b)->UID2;
   return (a_f - b_f);
}

int main(int argc, char *argv[]) 
{
    const char *usage_msg = "Usage: disk_sort <name of the input file> <total " 
        "mem in bytes> <block_size>\n";
    char *input_file;
    int total_mem = 0;
    int block_size = 0;
    int recs_per_block = 0;

    if (argc != 4)
    {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        total_mem = (int) strtol(argv[2], (char **)NULL, 10);
        block_size = (int) strtol(argv[2], (char **)NULL, 10);

        if(block_size % rec_size != 0)
        {
            printf("%d is not a valid block size. Must be a multiple of 8.\n",
                block_size);
            exit(1);
        } else
        {
            recs_per_block = block_size / rec_size;
        }
    }

    FILE *file = fopen(input_file, "rb");
    if (!file)
    {
        printf("Could not open file for reading. Exiting.\n");
        exit(1);
    }

    int file_size = find_file_size(file);

    // Find number of blocks, each of size block_size that memory can hold
    int blocks_in_mem = total_mem/block_size;
    // Divide the file into num_chunks chunks
    int num_chunks = file_size/(blocks_in_mem * block_size);

    /* If the file_size isn't perfectly divisible by blocks_in_mem, add
     * an extra chunk (note how chunks are equivalent to the number of
     * "runs" we must do. 
     */ 
    int is_leftover_bytes = file_size % (blocks_in_mem * block_size);
    if (is_leftover_bytes != 0) 
    {
        num_chunks = num_chunks + 1;
    }

    if ((num_chunks*block_size) > total_mem)
    {
        printf("Not enough memory to perform 2PMMS. Exiting\n");
        fclose(file);
        exit(1);
    }

    // Total number of records that can be held in memory at one time
    int total_recs_in_mem = blocks_in_mem * recs_per_block; 
    int curr_run = 0;
    // Phase 1
    // Up to 10 characters for sortedXXX\0
    char *filename = calloc(10, sizeof(char));
    strncpy(filename, "sorted", 6); 
    while (curr_run < num_chunks) 
    {
        char file_num[4];
        sprintf(file_num, "%d", curr_run);
        // Create a new file to write to 
        strncpy(filename + 6, file_num, 1 * sizeof(char));
         
        FILE *file_write = fopen(filename, "wb");
        if (!file_write) 
        {
            printf("Could not open file for writing. Exiting.\n");
            free(filename);
            fclose(file);
            exit(1);
        }

        // Allocate a buffer that is a multiple of block_size
        Record *buffer = (Record *) calloc(total_recs_in_mem, rec_size);
        if (curr_run != (num_chunks - 1) || (is_leftover_bytes == 0)) 
        {
            handle_fread_fwrite(total_recs_in_mem, "fread", buffer, rec_size, total_recs_in_mem, file); 
        } else if ((curr_run == (num_chunks - 1)) && (is_leftover_bytes != 0))
        {
            // If there are some leftover bytes to read, read them. 
            /* Note to self: fread is called without handle_fread_fwrite
             * because we don't know how many bytes to expect.
             */ 
            int bytes_read = fread(buffer, rec_size, total_recs_in_mem, file);
            if (bytes_read == 0)
            {
                printf("fread returned 0 bytes when we expected to read some"
                    "bytes. Exiting.\n");
                free(filename);
                free(buffer);
                fclose(file);
                exit(1);
            } 
        }
        // Quick sort the buffer (Phase I)
        qsort(buffer, total_recs_in_mem, rec_size, compare);

        fwrite(buffer, rec_size, total_recs_in_mem, file_write); 
         
        fclose(file_write);
        free(buffer);
        curr_run = curr_run + 1;
    }

    free(filename);
    fclose(file);

    // TODO: mergesort the different chunks we have (req of 1.2 Producing sorted runs) - Phase I of 2PMMS
    
    return 0;     
}
