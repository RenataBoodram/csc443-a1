#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "merge.h"

int main(int argc, char *argv[]) {
    const char *usage_msg = "Usage: q1 <input file>\n";
    char *orig_input_file;

    if (argc != 2) 
    {
        printf("%s", usage_msg);
        exit(1);
    } else {
        orig_input_file = argv[1];
    }

    //int num_chunks = 0; 
    
    // Sort the file by UID1
    disk_sort(orig_input_file, 1, 0);

    // Delete intermediate sorted files

    char cmd[50];
    strcpy( cmd, "rm -f sorted*") ;
    int ret_val = system(cmd);
    if (ret_val == -1)
    {
        perror("Failed to remove intermediate sort files. Exiting.");
        exit(1);
    }

    // Sort the file by UID2
    // Pass in 1 to run Q1
    disk_sort(orig_input_file, 2, 1);
    ret_val = system(cmd);
    strcpy(cmd, "rm -f entire_sorted*");
    ret_val = system(cmd);




    // Uncomment for testing - when entire_sorted1 and entire_sorted2 have already 
    // been produced in a prior run
/*
    MergeManager *manager = (MergeManager *)malloc(sizeof(MergeManager));
    manager->heap_capacity = 2;
    manager->heap = (HeapElement *)calloc(2, sizeof(HeapElement));
    query_one_setup(manager, 2);
*/

    
    return 0;
}
