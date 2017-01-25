// 2.3 Sequential Reads

// TODO
// 1.read binary record file in blocks
// 2. return the user who is following the most amount of people [max]
// 3. return the average of each users following

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "read_ram_seq.h"

int main(int argc, char *argv[])
{
    const char *usage_msg = "Usage: read_ram_seq <filename><block_size>\n";
    char *input_file;
    int block_size = 0;
 
    if (argc != 3) {
        printf("%s", usage_msg);
        exit(1);
    } 
    else {
        input_file = argv[1];
        block_size = (int) strtol(argv[2], (char **)NULL, 10);

        if(block_size % rec_size != 0){
            printf("%s.\n", "Not a valid block size");
            exit(1);
        }
        else{
        int records_per_block = block_size / rec_size;
        printf("RECORDS_PER_BLOCK %d\n", (int)records_per_block);
        }
    }

    FILE *file = fopen(input_file, "r");
    ssize_t read;
    char *line = NULL;
    size_t n = 0;
    
    int total_follows = 0;
    int unique_users = 0;

    int max_user_follow_count = 0;
    int cur_user_follow_count = 0;
    //need this to identify unique users
    int cur_user_id = 0;

    float avg_num_followed = 0; 

    if (file){
        while ((read = getline(&line, &n, file)) != -1){
            // printf("Current line: %s", line);    
            Record *rec = line_to_record(line);
            // printf("Record uid1: %d, uid2: %d\n", rec->uid1, rec->uid2);

     		//if new user, then increment unqiue users 
     		//and reset follower counter back to 1
     		if(cur_user_id != rec->uid1){
     			cur_user_id = rec->uid1;
     			unique_users++;
				cur_user_follow_count = 1;
     		}
     		else{
     			cur_user_follow_count++;
    		}

			if(cur_user_follow_count > max_user_follow_count){
				max_user_follow_count = cur_user_follow_count;
			}

            total_follows++;
        } 
    } 
    else {
        printf("File %s could not be found. Exiting.\n", input_file);
        exit(1);
    }

    avg_num_followed = (float)total_follows/unique_users;
    fclose(file);

    printf("Parsed %d, lines of data. \n", total_follows);
    printf("The most people followed is: %d. \n", max_user_follow_count);
    printf("Average number of follows is: %.2f. \n", avg_num_followed);
    return 0;
}

Record *line_to_record(char *line) {
    const char *delim = ",";
    Record *rec = malloc(sizeof(Record));
    int line_len = strlen(line);
    char *line_copy = malloc(sizeof(char) * line_len);
    strncpy(line_copy, line, line_len); 

    // Initialize str_arr where each string has max len equal to total line
    // len excluding the comma, divded by 2 for 2 IDs.
    //char str_arr[2][(line_len - 1)/2];
    char *str_arr[2];
    char *val = strtok(line_copy, delim);
    int i = 0;
    while (val != NULL) {
        str_arr[i] = val;
        val = strtok(NULL, delim);
        i++;
    }

    rec->uid1 = (int) strtol(str_arr[0], (char **)NULL, 10);
    rec->uid2 = (int) strtol(str_arr[1], (char **)NULL, 10);
    free(line_copy);
    return rec;
}

// Record *buffer = (Record *) calloc (records_per_block, sizeof(Record));
//     int result = fread(buffer,sizeof(Record),records_per_block,fp_read);
//     int i;
    
//     for(i = 0; i < 27; i++){
//         fread(buffer,sizeof(Record),records_per_block,fp_read);
//         printf("%d\n",buffer->uid1);
//     }