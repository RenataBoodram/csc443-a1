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
   int a_f = ((const struct Record*)a)->f;
   int b_f = ((const struct Record*)b)->f;
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
        block_size = (int) strtol(argv[2], (char **)NULL, 10);

        if(block_size % rec_size != 0)
        {
            printf("%d is not a valid block size. Must be a multiple of 8.\n",
                block_size);
            exit(1);
        } else
        {
            records_per_block = block_size / rec_size;
        }
    }

    FILE *file = fopen(input_file, "rb");
    if (!file)
    {
        printf("Could not open file for reading. Exiting.\n");
        exit(1);
    }

    // 1.1 Main-memory sorting

    int file_size = find_file_size(file);

    int recs_per_block = block_size/rec_size;
    // TODO: put code below into a loop
    // TODO: figure out what to multiply buffer size by
    //Record *buffer = (Record *) calloc(, rec_size) 
    qsort (buffer, total_records, sizeof(Record), compare);
    // Write something
    // Flush buffer
    
     
}
