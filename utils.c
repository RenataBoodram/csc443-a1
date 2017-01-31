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

/*
 * Returns size of file. Terminates on error. 
 */
int find_file_size(FILE *file)
{
    int file_size = -1;
    handle_fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    if (file_size == -1) 
    {
        printf("Error when determining file size. Exiting.\n");
	fclose(file);
	exit(1);
    }

    return file_size;
}

/*
 * Update max value and other variables required to calculate the avg where
 * avg = total_follows/unique_users.
 */

void get_max_avg(Record *buffer, int cond, int *max, int *unique_users,
    int *total_follows, int *cur_user_id, int *cur_user_follow_count)
{
    /* Note: cur_user_id keeps track of the current ID
     *       cur_user_follow_count keeps track of the number of followers for
     *       the current user.
     */
    int i = 0;
    while (i < cond)
    {
        printf("READ: %d, %d\n", buffer[i].uid1, buffer[i].uid2);

        // Update unique_users when a new ID has been encountered
        if (*cur_user_id != buffer[i].uid1)
	{
	    *cur_user_id = buffer[i].uid1;
	    *unique_users = *unique_users + 1;
            // Reset the follow count of the current user to 1
	    *cur_user_follow_count = 1;
	} else {

            /* If the same user is being tracked, keep track of their
             * follow count.
             */ 
	    *cur_user_follow_count = *cur_user_follow_count + 1;
	}

        /* If the current user has a higher follow count, it is the
         * new max.
         */ 
	if (*cur_user_follow_count > *max)
	{
	    *max = *cur_user_follow_count;
	}

        // Increase the total number of follows (for all users)
	*total_follows = *total_follows + 1;
        i++;
    }

}

/* Prints total data rate time. Prints avg and max values if non-zero. */
void print_vals(struct timeb t_begin, struct timeb t_end, int total_bytes, int max, float avg)
{
    if (max != 0)
    {
        printf("Max: %d\n", max);
    }
    if (avg != (float) 0) 
    {
        printf("Average: %.2f\n", avg);
    }
    long time_spent_ms;
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
        + (t_end.millitm - t_begin.millitm));
    printf ("Data rate: %.3f MBPS\n",
        (total_bytes/(float)time_spent_ms * 1000)/(1024*1024));
}
