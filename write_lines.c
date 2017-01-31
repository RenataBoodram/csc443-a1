#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "part1.h"

/*
 * Program:        write_lines
 * Description:    Read from a CSV file and write to a new CSV file.
 * Authors:        boodram8, phamtyle
 */

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: write_lines <input filename> " 
        "<output filename>\n";
    char *input_file;
    char *output_filename;
 
    if (argc != 3) {
        printf("%s", usage_msg);
        exit(1);
    } else {
        input_file = argv[1];
        output_filename = argv[2];
    }

    FILE *file = fopen(input_file, "r");
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
        int line_len = strlen(line);
        total_bytes += (long) strlen(line);
        handle_fread_fwrite(line_len, "fwrite", line, 1, line_len, output_file);
    } 
    ftime(&t_end);
    fclose(output_file);
    fclose(file);

    print_vals(t_begin, t_end, total_bytes, 0, 0);
    return 0;
}
