#include <assert.h>
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

    if (argc != 4)
    {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        total_mem = (int) strtol(argv[2], (char **)NULL, 10);
        block_size = (int) strtol(argv[3], (char **)NULL, 10);

        if(block_size % rec_size != 0)
        {
            printf("%d is not a valid block size. Must be a multiple of 8.\n",
                block_size);
            exit(1);
        } 
    }

    FILE *file = fopen(input_file, "rb");
    if (!file)
    {
        printf("Could not open file for reading. Exiting.\n");
        exit(1);
    }

    int file_size = find_file_size(file);
    // Halve the total_mem to account for qsort
    total_mem = total_mem / 2;

    // Find number of blocks, each of size block_size that memory can hold
    int blocks_in_mem = total_mem/block_size;
    // Find number of times the file has to be partitioned
    int num_chunks = file_size/(blocks_in_mem * block_size);
    int is_leftover_bytes = file_size % (blocks_in_mem * block_size);
    // Account for leftover bytes if the file is not perfectly divisible
    if (is_leftover_bytes != 0)
    {
        num_chunks = num_chunks + 1; 
    }

    // +1 for output buffer
    if (((num_chunks + 1)*block_size) > total_mem)
    {
        printf("Not enough memory to perform 2PMMS. Exiting\n");
        fclose(file);
        exit(1);
    }

    // Total number of records that can be held in memory at one time
    int total_recs_in_mem = total_mem/rec_size; 
    int curr_run = 0;
    // Phase 1
    // Up to 10 characters for sortedXXX\0
    char *filename = calloc(14, sizeof(char));
    strncpy(filename, "sorted", 6); 
    while (curr_run < num_chunks) 
    {
        char file_num[4];
        sprintf(file_num, "%d", curr_run);
        // Create a new file to write to 
        strncpy(filename + 6, file_num, 1 * sizeof(char));
        strncpy(filename + 7, ".dat", 4 * sizeof(char));
         
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
            qsort(buffer, total_recs_in_mem, rec_size, compare);
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
            /* Only sort the number of records leftover (we don't want to sort
             * any 0 records.
             */
            qsort(buffer, bytes_read, 1, compare);
        }

        fwrite(buffer, rec_size, total_recs_in_mem, file_write); 
         
        fclose(file_write);
        free(buffer);
        curr_run = curr_run + 1;
    }

    free(filename);
    fclose(file);

    // Initialize MergeManager fields for Phase II
    MergeManager *manager = (MergeManager *)malloc(sizeof(MergeManager));
    int heap_cap = num_chunks;
    manager->heap_capacity = heap_cap;
    manager->heap = (HeapElement *)calloc(heap_cap, sizeof(HeapElement));

    int i = 0;
    int input_file_numbers[heap_cap];
    int current_input_file_positions[heap_cap]; // all 0s
    int current_input_buffer_positions[heap_cap]; // all 0s 
    int total_input_buffer_elements[heap_cap];

    Record **input_buffers = calloc(heap_cap, rec_size);
    int recs_per_block = block_size/rec_size;
    // +1 to account for output buffer space
    int recs_per_buffer = recs_per_block * (blocks_in_mem / (num_chunks + 1));
    manager->input_buffer_capacity = recs_per_buffer;

    while (i < heap_cap)
    {
        input_file_numbers[i] = i;
        current_input_file_positions[i] = 0;
        current_input_buffer_positions[i] = 0;
        total_input_buffer_elements[i] = 0;
        input_buffers[i] = (Record *)calloc(manager->input_buffer_capacity, rec_size);
        i = i + 1;
    }
    manager->input_file_numbers = input_file_numbers; 
    manager->current_input_file_positions = current_input_file_positions;
    manager->current_input_buffer_positions = current_input_buffer_positions;
    manager->total_input_buffer_elements = total_input_buffer_elements;
    manager->input_buffers = input_buffers;


    // Initialize output fields
    manager->output_buffer_capacity = recs_per_buffer;
    // Check if there is an extra buffer available
    int num_extra_buff = blocks_in_mem % (num_chunks + 1);
    if (num_extra_buff != 0)
    {
        manager->output_buffer_capacity = recs_per_buffer + (num_extra_buff *recs_per_block);
    }
    manager->output_buffer = (Record *)calloc(manager->output_buffer_capacity, rec_size); 
    manager->current_output_buffer_position = 0; // Starts here

    strcpy(manager->output_file_name, "entire_sorted.dat");
    strcpy(manager->input_prefix, "sorted");

    manager->current_heap_size = 0;
    
    merge_runs(manager);
    
    return 0;     
}
