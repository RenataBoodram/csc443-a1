#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    ssize_t read; // Check status  of reading input file
    char *line = NULL; // Holds line of input file
    size_t n = 0;

    if (file) 
    {
        while ((read = getline(&line, &n, file)) != -1) 
        {
            if (output_file)
            {
                printf("Writing line %s\n", line);
                printf("SIZE OF LINE: %zu\n", strlen(line));
                fwrite(line, strlen(line), 1, output_file);
                      
            } else {
                printf("File %s could not be opened for writing. Exiting.\n", 
                       output_filename);
                exit(1);
            }
        } 
    } else {
        printf("File %s could not be found. Exiting.\n", input_file);
        exit(1);
    }

    fclose(output_file);
    fclose(file);
    return 0;
}

/* Returns array of strings given a file line. */
void parse_line(char *line, char *str_arr[]) {
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
