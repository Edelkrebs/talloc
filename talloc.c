#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct{
    uint8_t free : 1; // 1 = FREE, 0 = ALLOCATED
    uint64_t size : 63;
    void* next;
} __attribute__((packed)) entry_header;

typedef struct{
    uint64_t* memory_start;
    uint64_t memsize;
    entry_header* first_header;
} heap_t;

heap_t heap;

void* allocate(uint64_t size){
    entry_header* current_header = (entry_header*)heap.first_header;
    uint64_t index = 0;

    for(entry_header* current_header = (entry_header*)heap.first_header; current_header; current_header = current_header->next){
        if(current_header->size >= size + sizeof(entry_header) && current_header->free == 1){
            entry_header hdr = {.free = 1, .next = current_header->next, .size = current_header->size - size - sizeof(entry_header)};
            entry_header* hdr_ptr = (entry_header*)((void*)current_header + size + sizeof(entry_header));
            *hdr_ptr = hdr;
            current_header->free = 0;
            current_header->next = (void*)hdr_ptr;
            current_header->size = size;
            return;
        } 
        index++;
    }
    printf("COULDNT ALLOCATE MEMORY!\n");
    exit(9);

    return 0;
}

void free(void* ptr){
    entry_header* current_header = (entry_header*)heap.first_header;
    entry_header* prev_header = 0;

    for(entry_header* current_header = (entry_header*)heap.first_header; current_header; current_header = current_header->next){
        if(current_header == ptr){
            if(prev_header == 0){
                current_header->free = 1;
                return;
            }
            prev_header->size += current_header->size + sizeof(entry_header);
            prev_header->next = current_header->next;
        }
        prev_header = current_header;
    }
    printf("COULDNT FREE POINTER");
    exit(-1);
}

void init_heap(uint64_t heap_size){
    entry_header first_hdr = {.free = 1, .size = heap_size - sizeof(entry_header), .next = 0};

    heap_t start_heap = {.memory_start = (uint64_t*) malloc(heap_size), .memsize = heap_size};

    start_heap.first_header = (entry_header*)start_heap.memory_start;
    *start_heap.first_header = first_hdr;

    heap = start_heap;
}

void main(void){

    init_heap(0x1000);

}
