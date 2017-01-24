#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "part1.h"

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: write_blocks_seq <input filename> <block \
                             size>. Note: block size should be entered in bytes.\n";
    char *input_file;
    int block_size = 0;
    int records_per_block = 0;
 
    if (argc != 3) {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        block_size = (int) strtol(argv[2], (char **)NULL, 10);
        records_per_block = block_size / rec_size;
        printf("RECORDS_PER_BLOCK %d\n", records_per_block);
    }

    FILE *file = fopen(input_file, "r");
    FILE *output_file;  
    const char *output_filename = "records.dat";
    ssize_t read; // Check status  of reading input file
    char *line = NULL; // Holds line of input file
    size_t n = 0;

    Record *buffer = (Record *) calloc(records_per_block, rec_size);
    int total_records = 0; // Total records written so far
    if (file) 
    {
        while ((read = getline(&line, &n, file)) != -1) 
        {
            // Check if buffer is full
            if (total_records == records_per_block)
            {
                if (!(output_file = fopen(output_filename, "wb" )))
                {
                    printf("Binary file records.dat could not be opened for \
                            writing. Exiting.");
                    fclose(file);
                    free(buffer);
                    exit(1);
                } else { // Write buffer to the binary *.dat file
                    printf("Writing buffer...\n");
                    fwrite(buffer, rec_size, total_records, output_file);
                    fflush(output_file);               
                    total_records = 0;
                }  
            }
            printf("Current line: %s", line);    
            char *str_arr[2];
            parse_line(line, str_arr);
            Record *rec = line_to_record(str_arr);
            printf("Record uid1: %d, uid2: %d\n", rec->uid1, rec->uid2);
            memcpy(buffer + (total_records * rec_size), (const void *) rec, 
                   rec_size);
            free(rec);

            total_records++;
        } 
    } else {
        printf("File %s could not be found. Exiting.\n", input_file);
        exit(1);
    }

    fclose(output_file);
    free(buffer);
    fclose(file);
    return 0;
}

/* Returns array of strings given a file line. */
void parse_line(char *line, char *str_arr[])
{
    const char *delim = ",";
    char *val = strtok(line, delim);

    int i = 0;
    while (val)
    {
        str_arr[i] = val;
        val = strtok(NULL, delim);
        i++;
    }
}

Record *line_to_record(char *str_arr[]) {
    Record *rec = malloc(rec_size);
    rec->uid1 = (int) strtol(str_arr[0], (char **)NULL, 10);
    rec->uid2 = (int) strtol(str_arr[1], (char **)NULL, 10);
    return rec;
}
