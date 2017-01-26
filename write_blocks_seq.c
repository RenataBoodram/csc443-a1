#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "part1.h"

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: write_blocks_seq <input filename> <block \
                             size in bytes>.\n";
    char *input_file; // Input filename
    int block_size = 0;
    int records_per_block = 0;
 
    if (argc != 3) {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        block_size = (long) strtol(argv[2], (char **)NULL, 10);
        //printf("BLOCK SIZE: %d\n", block_size);
        records_per_block = block_size / rec_size;
        //printf("RECORDS_PER_BLOCK %d\n", records_per_block);
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

    ssize_t read; // Check status  of reading input file
    char *line = NULL; // Holds line of input file
    size_t n = 0;
    struct timeb t_begin, t_end;

    Record *buffer = (Record *) calloc(records_per_block, rec_size);
    int recs_so_far = 0; // Total records written so far
    long total_records = 0;
    ftime(&t_begin);
    while ((read = getline(&line, &n, file)) != -1) 
    {
        // Remove end-of-line characters
        line [strcspn (line, "\r\n")] = '\0';
        // Check if buffer is full
        if (recs_so_far == records_per_block)
        {
            printf("Writing buffer...\n");
            fwrite(buffer, rec_size, total_records, output_file);
            fflush(output_file);               
            recs_so_far = 0;
        }
        printf("Current line: %s", line);    
        char *str_arr[2];
        parse_line(line, str_arr);
        Record *rec = line_to_record(str_arr);
        //printf("Record uid1: %d, uid2: %d\n", rec->uid1, rec->uid2);
        memcpy(buffer + (recs_so_far * rec_size), (const void *) rec, 
            rec_size);
        free(rec);
        recs_so_far++;
        total_records++;
    } 

    fclose(output_file); 
    fclose(file);
    ftime(&t_end);
    free(buffer);

    long time_spent_ms;
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
        + (t_end.millitm - t_begin.millitm));
    printf ("Data rate: %.3f BPS\n", ((total_records*rec_size)/(float)time_spent_ms * 1000));

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

Record *line_to_record(char *str_arr[])
{
    Record *rec = malloc(rec_size);
    rec->uid1 = (int) strtol(str_arr[0], (char **)NULL, 10);
    rec->uid2 = (int) strtol(str_arr[1], (char **)NULL, 10);
    return rec;
}
