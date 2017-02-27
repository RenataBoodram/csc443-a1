#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "part1.h"

/*
 * Program:        write_blocks_seq 
 * Description:    Read from a CSV file up to specified block size before
 *                 writing buffer of blocks to records.dat
 * Authors:        boodram8, phamtyle
 */
int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: write_blocks_seq <input filename> <block "
                             "size in bytes>.\n";
    char *input_file; // Input filename
    int block_size = 0;
    int records_per_block = 0;
 
    if (argc != 3) {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        block_size = (long) strtol(argv[2], (char **)NULL, 10);
	if ((block_size % rec_size) != 0)
	{
	    printf("Block size %d is not a multiple of record size %d."
	        "Exiting.\n", block_size, rec_size);
            exit(1);
	}
        records_per_block = block_size / rec_size;
    }
    const char *output_filename = "records.dat"; 
    FILE *file = fopen(input_file, "r");
    FILE *output_file = fopen(output_filename, "wb");

    if (!file) 
    {
        printf("Input file could not be opened for reading. Exiting.\n");
        exit(1);
    }
    if (!output_file) 
    {
        printf("Output file could not be opened for writing. Exiting.\n");
        fclose(file);
        exit(1);
    }

    //ssize_t read; // Check status  of reading input file
    char *curr_line;
    char line[1024]; // Holds line of input file
    //size_t n = 0;
    struct timeb t_begin, t_end;
    Record *buffer = (Record *) calloc(records_per_block, rec_size);
    //Record *rec = malloc(rec_size);
    int recs_so_far = 0; // Total records written so far
    long total_records = 0;
    ftime(&t_begin);
    while ((curr_line = fgets(line, 1024, file)) != NULL)
    {
        // Remove end-of-line characters
        curr_line [strcspn (curr_line, "\r\n")] = '\0';
        // Check if buffer is full
        if (recs_so_far == records_per_block)
        {
            handle_fread_fwrite(recs_so_far, "fwrite", buffer, rec_size, 
                recs_so_far, output_file);
            recs_so_far = 0;
        }
        char *str_arr[2];
        parse_line(curr_line, str_arr);
        //line_to_record(rec, str_arr);
        //memcpy(buffer + recs_so_far, (const void *) rec, 
        //    rec_size);
        buffer[recs_so_far].uid1 = (int) strtol(str_arr[0], (char **)NULL, 10);
        buffer[recs_so_far].uid2 = (int) strtol(str_arr[1], (char **)NULL, 10); 
        recs_so_far++;
        total_records++;
    }
    
    // Write remaining buffer
    if (recs_so_far >= 1)
    {
        handle_fread_fwrite(recs_so_far, "fwrite", buffer, rec_size, recs_so_far,
            output_file);
    }
    ftime(&t_end);
    fclose(output_file); 
    fclose(file);
    //free(rec);
    free(buffer);

    print_vals(t_begin, t_end, (total_records * rec_size), 0, 0);

    return 0;
}
