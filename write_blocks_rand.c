#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>
#include "part1.h"

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: write_blocks_rand <input filename> "
        "<X number of updates>\n";
    char *input_file; // Input filename
    int num_updates = 0;
 
    if (argc != 3) {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        num_updates = (int) strtol(argv[2], (char **)NULL, 10);
    }

    FILE *file = fopen(input_file, "r+b");

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

    // Total number of records in file
    int num_of_recs = file_size / rec_size;
    
    srand(time(NULL)); // Initialize random seed
    int i = 0;
    long rec_num = 0;
    Record *rec = malloc(rec_size);
    rec->uid1 = 1;
    rec->uid2 = 2;
    struct timeb t_begin, t_end;

    ftime(&t_begin);
    while (i < num_updates)
    {
        // Choose a random record number in interval [0, num_of_recs - 1].
        rec_num = rand() % num_of_recs; 
    
        printf("Record %ld chosen\n", rec_num);
        // Seek to the start of the record
        handle_fseek(file, (rec_num * rec_size), SEEK_SET);
        // Write the dummy record
        handle_fread_fwrite(rec_size, "fwrite", rec, 1, rec_size, file);
        
        i++;
    }
    ftime(&t_end);
    free(rec);
    fclose(file);
    long time_spent_ms;
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
        + (t_end.millitm - t_begin.millitm));
    printf ("Data rate: %.3f BPS\n", 
        ((num_updates*rec_size)/(float)time_spent_ms * 1000));

    return 0;
}   
