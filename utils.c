#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "part1.h"

/* Returns array of strings given a file line delimited by commas. */
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

/* Creates Record given array of Record fields. */
Record *line_to_record(char *str_arr[])
{
    Record *rec = malloc(rec_size);
    rec->uid1 = (int) strtol(str_arr[0], (char **)NULL, 10);
    rec->uid2 = (int) strtol(str_arr[1], (char **)NULL, 10);
    return rec;
}

/* Wrapper function to handle fread and fwrite errors. */
void handle_fread_fwrite(int bytes, const char *func, void *ptr, size_t size,
    size_t nmemb, FILE *stream)
{
    int bytes_used = -1;
    if (strncmp(func, "fread", 5) == 0)
    {
        bytes_used = fread(ptr, size, nmemb, stream);
    } else if (strncmp(func, "fwrite", 6) == 0) {
        bytes_used = fwrite(ptr, size, nmemb, stream);
    }

    if ((bytes_used != -1) && ((bytes_used != bytes) || (bytes_used == 0)))
    {
        printf("%s returned %d bytes when %d was expected. Exiting.\n", 
            func, bytes_used, bytes);
        free(ptr);
        fclose(stream);
        exit(1);
    }

}

/* Wrapper function to handle fseek errors. */
void handle_fseek(FILE *stream, long offset, int whence)
{
    int result = fseek(stream, offset, whence);
    if (result == -1)
    {
        printf("fseek failed to seek in file by %ld offset.\n", offset); 
    }
}
