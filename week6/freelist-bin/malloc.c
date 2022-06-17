//
// >>>> malloc challenge! <<<<
//
// Your task is to improve utilization and speed of the following malloc
// implementation.
// Initial implementation is the same as the one implemented in simple_malloc.c.
// For the detailed explanation, please refer to simple_malloc.c.

// !! this code took 11minutes to finish challenge #1
// !! 1 minute to finish #2 & #3
// !! 1 minute to #4

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Interfaces to get memory pages from OS
//

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

//
// Struct definitions
//

// 空いている領域の大きさと次の空いている領域へのポインタ
typedef struct my_metadata_t {
  size_t size;
  struct my_metadata_t *next;
} my_metadata_t;

// 空いている領域をつなぐリスト
typedef struct my_heap_t {
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;

//
// Static variables (DO NOT ADD ANOTHER STATIC VARIABLES!)
//
// There are four freelist bins
my_heap_t my_heaps[4];

//
// Helper functions (feel free to add/remove/edit!)
//

void my_add_to_free_list(my_metadata_t *metadata) {
  assert(!metadata->next);

  my_heap_t *my_heap;
  if (metadata->size <= 256) my_heap = &my_heaps[0];
  else if (metadata->size <= 512) my_heap = &my_heaps[1];
  else if (metadata->size <= 1024) my_heap = &my_heaps[2];
  else my_heap = &my_heaps[3];

  metadata->next = my_heap->free_head;
  my_heap->free_head = metadata;
}

void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev, my_heap_t *my_heap) {
  if (prev) {
    prev->next = metadata->next;
  } else {
    my_heap->free_head = metadata->next;
  }
  metadata->next = NULL;
}

//
// Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
//

// This is called at the beginning of each challenge.
void my_initialize() {
  for(int i=0; i<4; i++) {
    my_heaps[i].free_head = &my_heaps[i].dummy;
    my_heaps[i].dummy.size = 0;
    my_heaps[i].dummy.next = NULL;
  }
}

// my_malloc() is called every time an object is allocated.
// |size| is guaranteed to be a multiple of 8 bytes and meets 8 <= |size| <=
// 4000. You are not allowed to use any library functions other than
// mmap_from_system() / munmap_to_system().
/* ほしいサイズを受け取って、そのサイズ分の領域を確保し、その領域の先頭アドレスを返す */
void *my_malloc(size_t size) {
  // サイズによってどのビンを扱うか分ける
  my_heap_t *my_heap;
  int heap_idx;
  if (size <= 256) heap_idx = 0;
  else if (size <= 512) heap_idx = 1;
  else if (size <= 1024) heap_idx = 2;
  else heap_idx = 3;

  my_metadata_t *metadata;
  my_metadata_t *prev;
  my_metadata_t *best_metadata;
  my_metadata_t *best_prev;

  for(; heap_idx < 4; heap_idx++) {
    my_heap = &my_heaps[heap_idx];

  // 空き領域の先頭
  metadata = my_heap->free_head;
  prev = NULL;
  // Best-fit: Find the smallest free slot the object fits.
  my_metadata_t initial;
  initial.size = SIZE_MAX;
  initial.next = NULL;
  best_metadata = &initial;
  best_prev = NULL;
// printf("debug00\n");
  while (metadata) {
    if (metadata->size >= size && metadata->size < best_metadata->size) {
      best_metadata = metadata;
      best_prev = prev;
      if(metadata->size == size) break;
    }
    prev = metadata;
    metadata = metadata->next;  
  }

  if(best_metadata->size!=SIZE_MAX || heap_idx==3) break;
  else continue;
  }
  // now, best_metadata points to the best-fit free slot
  // metadata points to NULL
  // and best_prev is the previous entry of best_metadata.
  
// printf("best_metadata->next: %p\n", best_metadata->next);
// printf("debug01\n");
  
  if (best_metadata->size == SIZE_MAX) {
// printf("debug02\n");
    // There was no free slot available. We need to request a new memory region
    // from the system by calling mmap_from_system().
    //
    //     | metadata | free slot |
    //     ^
    //     metadata
    //     <---------------------->
    //            buffer_size
    size_t buffer_size = 4096;
    my_metadata_t *metadata = (my_metadata_t *)mmap_from_system(buffer_size);
    metadata->size = buffer_size - sizeof(my_metadata_t);
    metadata->next = NULL;
    // Add the memory region to the free list.
    my_add_to_free_list(metadata);
    // Now, try my_malloc() again. This should succeed.  
    return my_malloc(size);
  }
// printf("debug03\n");

  // best_metadata, best_prev の名前を変更する
  metadata = best_metadata;
  prev = best_prev;

  // |ptr| is the beginning of the allocated object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  // Remove the free slot from the free list.
  void *ptr = metadata + 1;
  my_remove_from_free_list(metadata, prev, &my_heaps[heap_idx]);
  size_t remaining_size = metadata->size - size;
  metadata->size = size;

// printf("debug05\n");
// print_free_list();

  if (remaining_size > sizeof(my_metadata_t)) {
  // printf("debug: remaining_size\n");
    // Create a new metadata for the remaining free slot.
    //
    // ... | metadata | object | metadata | free slot | ...
    //     ^          ^        ^
    //     metadata   ptr      new_metadata
    //                 <------><---------------------->
    //                   size       remaining size
    my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
// printf("ptr: %p\n", ptr);
// printf("size: %ld\n", size);
// printf("new_metadata: %p\n", new_metadata);
    new_metadata->size = remaining_size - sizeof(my_metadata_t);
    new_metadata->next = NULL;
    // Add the remaining free slot to the free list.
    my_add_to_free_list(new_metadata);
  }
  
// printf("debug06\n");
  
  return ptr;
}

// This is called every time an object is freed.  You are not allowed to
// use any library functions other than mmap_from_system / munmap_to_system.
void my_free(void *ptr) {
  // Look up the metadata. The metadata is placed just prior to the object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;
  // Add the free slot to the free list.
  my_add_to_free_list(metadata);
}

// This is called at the end of each challenge.
void my_finalize() {
  // Nothing is here for now.
  // feel free to add something if you want!
}

void test() {
  // Implement here!
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
