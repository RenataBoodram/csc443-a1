#include "merge.h"

//manager fields should be already initialized in the caller
int merge_runs (MergeManager * merger){	
	int  result; //stores SUCCESS/FAILURE returned at the end	
	
	//1. go in the loop through all input files and fill-in initial buffers
	if (init_merge (merger)!=SUCCESS)
		return FAILURE;

	while (merger->current_heap_size > 0) { //heap is not empty
		HeapElement smallest;
		Record next; //here next comes from input buffer
		
		if(get_top_heap_element (merger, &smallest)!=SUCCESS)
			return FAILURE;

		result = get_next_input_element (merger, smallest.run_id, &next);
		
		if (result==FAILURE)
			return FAILURE;

		if(result==SUCCESS) {//next element exists, may also return EMPTY
			if(insert_into_heap (merger, smallest.run_id, &next)!=SUCCESS)
				return FAILURE;
		}		


		merger->output_buffer [merger->current_output_buffer_position].UID1=smallest.UID1;
		merger->output_buffer [merger->current_output_buffer_position].UID2=smallest.UID2;
		
		merger->current_output_buffer_position++;

        //staying on the last slot of the output buffer - next will cause overflow
		if(merger->current_output_buffer_position == merger-> output_buffer_capacity ) {
			if(flush_output_buffer(merger)!=SUCCESS) {
				return FAILURE;			
				merger->current_output_buffer_position=0;
			}	
		}
	
	}

	
	//flush what remains in output buffer
	if(merger->current_output_buffer_position > 0) {
		if(flush_output_buffer(merger)!=SUCCESS)
			return FAILURE;
	}
	
	clean_up(merger);
	return SUCCESS;	
}


int get_top_heap_element (MergeManager * merger, HeapElement * result){
	HeapElement item;
	int child, parent;

	if(merger->current_heap_size == 0){
		printf( "UNEXPECTED ERROR: popping top element from an empty heap\n");
		return FAILURE;
	}

	*result=merger->heap[0];  //to be returned

	//now we need to reorganize heap - keep the smallest on top
	item = merger->heap [--merger->current_heap_size]; // to be reinserted 

	parent =0;
	while ((child = (2 * parent) + 1) < merger->current_heap_size) {
		// if there are two children, compare them 
		if (child + 1 < merger->current_heap_size && 
				(compare_heap_elements(&(merger->heap[child]),&(merger->heap[child + 1]))>0)) 
			++child;
		
		// compare item with the larger 
		if (compare_heap_elements(&item, &(merger->heap[child]))>0) {
			merger->heap[parent] = merger->heap[child];
			parent = child;
		} 
		else 
			break;
	}
	merger->heap[parent] = item;
	
	return SUCCESS;
}

int insert_into_heap (MergeManager * merger, int run_id, Record *input){

	HeapElement new_heap_element;
	int child, parent;

	new_heap_element.UID1 = input->UID1;
	new_heap_element.UID2 = input->UID2;
	new_heap_element.run_id = run_id;
	
	if (merger->current_heap_size == merger->heap_capacity) {
		printf( "Unexpected ERROR: heap is full\n");
		return FAILURE;
	}
  	
	child = merger->current_heap_size++; /* the next available slot in the heap */
	
	while (child > 0) {
		parent = (child - 1) / 2;
		if (compare_heap_elements(&(merger->heap[parent]),&new_heap_element)>0) {
			merger->heap[child] = merger->heap[parent];
			child = parent;
		} 
		else 
			break;
	}
	merger->heap[child]= new_heap_element;	
	return SUCCESS;
}


/*
** TO IMPLEMENT
*/

int init_merge (MergeManager * manager) {
    int i = 0;
    while (i < manager->heap_capacity)
    {
        char file_num[4];
        char *filename = calloc(14, sizeof(char));
        sprintf(file_num, "%d", manager->input_file_numbers[i]);
        strncpy(filename, manager->input_prefix, 6 * sizeof(char)); 
        strncpy(filename + 6, file_num, 1 * sizeof(char));
        strncpy(filename + 7, ".dat", 4 * sizeof(char));

        FILE *file = fopen(filename, "rb");
        if (!file) 
        {
             printf("Could not open file for reading.\n");
             free(filename);
             return FAILURE;
        }

        fseek(file, manager->current_input_file_positions[i]*rec_size, SEEK_SET);

        // Read in initial input buffer elements 
        int bytes_read = fread(manager->input_buffers[i], rec_size, manager->input_buffer_capacity, file);
        if (bytes_read <= 0)
        {
            manager->current_heap_size--;
        }
        // Change the position because bytes_read bytes have already been read
        manager->current_input_file_positions[i] = bytes_read;
        manager->total_input_buffer_elements[i] = bytes_read;
        // Insert
        int curr_buff_pos = manager->current_input_buffer_positions[i];
        insert_into_heap(manager, manager->input_file_numbers[i], &manager->input_buffers[i][curr_buff_pos]);

        fclose(file);
        free(filename);
        i = i + 1;
    }
    
    return SUCCESS;
}

/* Write the buffer and flush to disk. */
int flush_output_buffer (MergeManager * manager) {
    FILE *file_write = fopen(manager->output_file_name, "a");
    if (!file_write)
    {
        printf("Could not open file %s for writing.\n", manager->output_file_name);
        return FAILURE;
    }

    // Write the current output_buffer
    fwrite(manager->output_buffer, rec_size, manager->current_output_buffer_position, file_write);
    fflush(file_write);
    fclose(file_write);
    manager->current_output_buffer_position = 0;

    return SUCCESS;
}

int get_next_input_element(MergeManager * manager, int file_number, Record *result) {
    // If the position has moved to the end 
    if (manager->current_input_buffer_positions[file_number] == manager->total_input_buffer_elements[file_number]) 
    {
        int refill = refill_buffer(manager, file_number);
        if (refill != SUCCESS)
        {
             return FAILURE;
        } else {
             // Check if run is complete
             if (manager->current_input_file_positions[file_number] == -1)
             {
                 return EMPTY;
             }
        } 

    }
    int curr_pos = manager->current_input_buffer_positions[file_number];
    *result = manager->input_buffers[file_number][curr_pos];
    manager->current_input_buffer_positions[file_number] = manager->current_input_buffer_positions[file_number] + 1;
    return SUCCESS;
}

int refill_buffer (MergeManager * manager, int file_number) {
    FILE *file;
    char file_num[4];
    sprintf(file_num, "%d", manager->input_file_numbers[file_number]);
    char *filename = calloc(14, sizeof(char)); 
    strncpy(filename, "sorted", 6);
    strncpy(filename + 6, file_num, 1 * sizeof(char));
    strncpy(filename + 7, ".dat", 4 * sizeof(char));
    file = fopen(filename, "rb");
    if (!file) 
    {
        printf("File could not be opened for reading.\n");
        free(filename);
        return FAILURE; 
    }

    // Seek to correct position for file.
    fseek(file, manager->current_input_file_positions[file_number] * rec_size, SEEK_SET);
    // Read into the buffer associated with this run
    int bytes_read = fread(manager->input_buffers[file_number], rec_size, manager->input_buffer_capacity, file);
    if (bytes_read > 0)
    {
        manager->current_input_file_positions[file_number] += bytes_read;
        manager->total_input_buffer_elements[file_number] = bytes_read;
    } else if (bytes_read == 0)
    {
        // Run is complete so set it to -1
        manager->current_input_file_positions[file_number] = -1;
    } else {
        printf("Could not read from file.\n");
        free(filename);
        return FAILURE;
    }

    return SUCCESS;
}

void clean_up (MergeManager * merger) {
    free(merger->heap);
    int i = 0;
    // Free all input buffers
    while (i < merger->heap_capacity)
    {
        free(merger->input_buffers[i]);
        i = i + 1;
    }
    // Free output buffer
    free(merger->output_buffer); 
    // Free the merger manager itself
    free(merger);
}

int compare_heap_elements (HeapElement *a, HeapElement *b) {
    if (b->UID2 < a->UID2)
    {
        return 1;
    } 
    return 0;
}
