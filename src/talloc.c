#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

void *heap_start = NULL;

struct meta_block{
	void *data_block;
	int size;
	int used;
	struct meta_block* next;
};

void *current_location = NULL; //points to end of the last block allocated
void *current_data_block = NULL; //points to start of last block allocated
int bytes_allocated = 0; //num of bytes from heap start to current location

size_t metadata_size = sizeof(struct meta_block);

void split_block(struct meta_block* original, int sub_block_size){
	int total_block_size = sub_block_size + metadata_size;
	struct meta_block *new_block = (struct meta_block*) ((void *) original + total_block_size);
	new_block->used = 0;
	new_block->next = original->next;
	new_block->data_block = (void *) new_block + metadata_size;
	new_block->size = original->size - total_block_size;
	original->size = sub_block_size;
	original->next = new_block;
}

struct meta_block *find_first_fit(int size){
	struct meta_block *block = (struct meta_block*) heap_start;
	while(block != NULL){
		if((block->used == 0) && (block->size >= size)){
			block->used = 1;
		        return block;
		}
		block = block->next;
	}
	return NULL;
}

struct meta_block *find_best_fit(int size)
{
	int best_size = 8192; 
        struct meta_block *block = (struct meta_block*) heap_start;
        while(block != NULL){
                if((block->used == 0) && (block->size >= size)){
                        if(block->size - size < best_size){
				best_size = block->size - size;
			}
                }
                block = block->next;
        }
	if(best_size == 8192){
		return NULL;
	}
	block = (struct meta_block*) heap_start;
	while(block != NULL){
                if((block->used == 0) && (block->size >= size)){
                        if(block->size - size == best_size){
                                return block;
			}
                }
                block = block->next;
        }
        return NULL;
}

void set_prev_block(){
	if(current_data_block == NULL){
                current_data_block = current_location + metadata_size;
        }
        else{
                struct meta_block *prev_meta_block = current_data_block-metadata_size;
                prev_meta_block->next = current_data_block + prev_meta_block->size;
		current_data_block += prev_meta_block->size + metadata_size;
        }
}

void free_all(){
        if(heap_start != NULL){
                munmap(heap_start, 8192);
                heap_start = NULL;
		current_location = NULL;
		current_data_block = NULL;
		bytes_allocated = 0;
        }
}

void *talloc(int size){ 
	if(heap_start == NULL){
		heap_start = mmap(NULL, 8192, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		current_location = heap_start;
	}
	bytes_allocated += size + metadata_size;
	if(bytes_allocated > 8192){
		bytes_allocated -= size + metadata_size;
		struct meta_block *metadata = find_best_fit(size);
		if(metadata == NULL){
			printf("Failed to allocate dynamic memory: Not enough space\n");
			return NULL;
		}
		if(metadata->size - size > 1000){
			split_block(metadata, size);
		}
		metadata->used = 1;
		return metadata->data_block;
	}
	set_prev_block();
	struct meta_block *metadata = current_location;
	metadata->data_block = current_location + metadata_size;
	metadata->next = NULL;
	metadata->used = 1;
	metadata->size = size;
	//memcpy(current_location, &metadata, metadata_size);
	current_location += size + metadata_size;
	return metadata->data_block;
}

void free_last_block(){
	struct meta_block *test = current_data_block - metadata_size;
	if(test == heap_start){
		free_all();
		return;
	}
	current_location -= test->size + metadata_size;
	bytes_allocated -= test->size + metadata_size;
	struct meta_block *block = heap_start;
	while(block->next != test){
		block = block->next;
	}
	current_data_block -= block->size + metadata_size;
	block->next = NULL;
}

void merge_blocks(struct meta_block *block1, struct meta_block* block2){
	 block1->size = metadata_size + block1->size + block2->size;
         block1->next = block2->next;
}

void tinyfree(void *block){
	struct meta_block *test = block-metadata_size;
	if(test != heap_start){
		struct meta_block *prev_block = heap_start;
        	while(prev_block->next != test){
                prev_block = prev_block->next;
		}
		if(prev_block->used==0){
			merge_blocks(prev_block, test);
			if(prev_block->next == NULL){
				current_data_block = (void*) prev_block + metadata_size;
			}
			test = prev_block;
		}

	}
	if(test->next == NULL){
		free_last_block();
		return;
	}
	if(test->next->used == 0){
		merge_blocks(test, test->next);
	}
	test->used=0;
}
