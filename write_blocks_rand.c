#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>
#include "part1.h"

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: write_blocks_rand <input filename> \
        <X number of updates>\n";
    char *input_file; // Input filename
    int num_updates = 0;
 
    if (argc != 3) {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        num_updates = (int) strtol(argv[2], (char **)NULL, 10);
    }

    FILE *file = fopen(input_file, "rb");

    if (!file) 
    {
        printf("Input file could not be opened for reading. Exiting.\n");
        exit(1);
    }

    /* Calculate the size of the program. */
    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    rewind(file);
    int num_of_recs = file_size / rec_size;
    
    /* Generate (num_updates) number of random numbers from 0 to N - 2 where
     *  N is the number of records.
     */
    //int pos_arr[num_updates];
    srand(time(NULL)); // Initialize random seed
    int i = 0;
    long offset = 0;
    Record *rec = malloc(rec_size);
    while (i < num_updates)
    {
        offset = rand() % (num_of_recs - 1); 
        // Seek to the start of the record
        fseek(file, ((offset - 1) * rec_size), SEEK_SET);
        // Read one record
        int bytes_read = fread(rec, rec_size, 1, file);
        if (bytes_read != rec_size)
        {
            printf("Error! Read %d bytes instead of expected %d\n", 
                bytes_read, rec_size);
        }
        // Write the same record (first move the pointer back)
        fseek(file, -rec_size, SEEK_CUR);
        fwrite(rec, rec_size, 1, file);
        
        i++;
    }
    free(rec);
    fclose(file);

    return 0;
}   
