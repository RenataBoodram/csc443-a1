#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "part1.h"

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: write_lines <input filename>\n";
    char *input_file;
 
    if (argc != 2) {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
    }

    FILE *file = fopen(input_file, "r");
    const char *output_filename = "records.csv";
    FILE *output_file = fopen(output_filename, "w");

    if (!file)
    {
        printf("Input file could not be opened for reading. Terminating.\n");
        exit(1);
    }
    if (!output_file)
    {
        printf("Output file could not be opened for writing. Terminating\n");
        fclose(file);
        exit(1);
    }

    size_t read; // Check status  of reading input file
    char *line = NULL; // Holds line of input file
    size_t n = 0;
    
    struct timeb t_begin, t_end;
    long total_bytes = 0;
    
    ftime(&t_begin);
    while ((read = getline(&line, &n, file)) != -1) 
    {
        //line [strcspn (line, "\r\n")] = '\0'; // remove end-of-line characters
        total_bytes += (long) strlen(line);
        printf("Writing line %s", line);
        //printf("total_bytes: %ld\n", total_bytes);
        //printf("SIZE OF LINE: %zu\n", strlen(line));
        fwrite(line, strlen(line), 1, output_file);
    } 
    fclose(output_file);
    fclose(file);
    ftime(&t_end);

    long time_spent_ms;
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
        + (t_end.millitm - t_begin.millitm));
    printf("Data rate: %.3f BPS\n", (total_bytes/(float) time_spent_ms * 1000));
    return 0;
}
