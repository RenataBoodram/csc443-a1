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
    
    // Sort the file by UID1
    disk_sort(orig_input_file, 1);

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
    disk_sort(orig_input_file, 2);
    ret_val = system(cmd);

    // Open and scan 2 sorted files

    return 0;
}
