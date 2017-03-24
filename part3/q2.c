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

    // Sort the file by UID1 - it will write "out.dat" containing out-degrees
    disk_sort(orig_input_file, 1, 2);
    // Get out-degrees
    distribution("entire_sorted1", "inorout0", 0);

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
    // Sort the file by UID2 -  it will write "in.dat" containing in-degrees
    disk_sort(orig_input_file, 2, 2);
    // Get in-degree
    distribution("entire_sorted2", "inorout1", 1);
    ret_val = system(cmd);

    // Join based on 
    query_two_setup();
    disk_sort("temp.dat", 2, 3);

    strcpy(cmd, "rm -f inorout*");
    ret_val = system(cmd);
    strcpy(cmd, "rm -f entire_sorted*");
    ret_val = system(cmd);
    strcpy(cmd, "rm -f temp.dat");
    ret_val = system(cmd);
    

    return 0;
}

