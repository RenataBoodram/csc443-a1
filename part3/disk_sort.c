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
int compare_uid1 (const void *a, const void *b) 
{
    int a_uid1 = ((const Record*)a)->UID1;
    int a_uid2 = ((const Record*)a)->UID2;
    int b_uid1 = ((const Record*)b)->UID1;
    int b_uid2 = ((const Record*)b)->UID2;
    // Check if UID2s of the corresponding UID1 are sorted
    if (a_uid1 == b_uid1) {
        // Sort by UID2 if the UID1 is the same
        return (a_uid2 - b_uid2);
    }
    return (a_uid1 - b_uid1);
}

int compare_uid2 (const void *a, const void *b)
{
    int a_uid1 = ((const Record*)a)->UID1;
    int a_uid2 = ((const Record*)a)->UID2;
    int b_uid1 = ((const Record*)b)->UID1;
    int b_uid2 = ((const Record*)b)->UID2;

    // Also need to make sure UID2s are in correct order
    if (a_uid2 == b_uid2) {
        return (a_uid1 - b_uid1);
    }
    return (a_uid2 - b_uid2);
}

//int main(int argc, char *argv[]) 
int disk_sort(char *input_file, int sortby)
{
/*
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
*/
    // Hard code total amount of memory and block size
    int total_mem = 209715200;
    int block_size = 8192;
    FILE *file = fopen(input_file, "rb");
    if (!file)
    {
        printf("Could not open file for reading. Exiting.\n");
        exit(1);
    }

    int file_size = find_file_size(file);
    // Halve the total_mem to account for qsort - only needed for A1.2
//    total_mem = total_mem / 2;

    // Find number of blocks, each of size block_size that memory can hold
    int blocks_in_mem = total_mem/block_size;
    // Find number of times the file has to be partitioned
    int num_chunks = file_size/(blocks_in_mem * block_size);
    int is_leftover_bytes = file_size % (blocks_in_mem * block_size);
    // Total records in memory that can be held at one time
    int total_recs_in_mem = (blocks_in_mem * block_size)/rec_size;

    // Account for leftover bytes if the file is not perfectly divisible
    //int recs_last_block = 0;
    if (is_leftover_bytes != 0)
    {
        //recs_last_block = file_size % (blocks_in_mem); 
        num_chunks = num_chunks + 1; 
    }

    // +1 for output buffer
    if (((num_chunks + 1)*block_size) > total_mem)
    {
        printf("Not enough memory to perform 2PMMS. Exiting\n");
        fclose(file);
        exit(1);
    }

    int curr_run = 0;
    // Phase 1
    // Up to 10 characters for sortedXXX\0
    char *filename = calloc(15, sizeof(char));
    strncpy(filename, "sorted", 6); 
    while (curr_run < num_chunks) 
    {
        char file_num[3 * sizeof(int)];
        sprintf(file_num, "%d", curr_run);
        // Create a new file to write to 
        strncpy(filename + 6, file_num, 3 * sizeof(char));
        strncpy(filename + 10, ".dat", 4 * sizeof(char));
       
        
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
            if (sortby == 1)
            {
                qsort(buffer, total_recs_in_mem, rec_size, compare_uid1);
            } else {
                qsort(buffer, total_recs_in_mem, rec_size, compare_uid2);
            }
            fwrite(buffer, rec_size, total_recs_in_mem, file_write);
        } else if ((curr_run == (num_chunks - 1)) && (is_leftover_bytes != 0))
        {
            // If there are some leftover bytes to read, read them. 
            /* Note to self: fread is called without handle_fread_fwrite
             * because we don't know how many bytes to expect.
             */ 
            // bytes_read refers to the number of records that we read
            //int bytes_read = fread(buffer, rec_size, recs_last_block, file);
            int bytes_read = fread(buffer, rec_size, total_recs_in_mem, file);
            //printf("%d read\n", bytes_read);
            //printf("%d\n", total_recs_in_mem);
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
            if (sortby == 1) 
            {
                qsort(buffer, bytes_read, rec_size, compare_uid1);
            } else {
                qsort(buffer, bytes_read, rec_size, compare_uid2);
            }
            fwrite(buffer, rec_size, bytes_read, file_write);
        }
        //fwrite(buffer, rec_size, total_recs_in_mem, file_write); 
         
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

    if (sortby == 1) {
        strcpy(manager->output_file_name, "entire_sorted1.dat");
    } else {
        strcpy(manager->output_file_name, "entire_sorted2.dat");
    }
    strcpy(manager->input_prefix, "sorted");


    manager->current_heap_size = 0;
    manager->sortedby = sortby; // Added field to sort the heap by
    
    merge_runs(manager);
    
    return 0;     
}
