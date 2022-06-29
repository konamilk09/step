//
// >>>> malloc challenge! <<<<
// 
// ~ reverse ptr, more bins, treat large size special ~
// Your task is to improve utilization and speed of the following malloc
// implementation.
// Initial implementation is the same as the one implemented in simple_malloc.c.
// For the detailed explanation, please refer to simple_malloc.c.

// https://jamboard.google.com/d/1FMJW0ujEN5qdr9mKcpN1rVfcNkIQw-IEpc2XS1RPpZ8/viewer?f=0

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HALF_META_SIZE 8

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
// sizeof(my_metadata_t) == 16

// 空いている領域をつなぐリスト
typedef struct my_heap_t {
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;
// sizeof(my_heap_t) == 24

//
// Static variables (DO NOT ADD ANOTHER STATIC VARIABLES!)
//
// There are four free list bins
my_heap_t my_heaps[34];

//
// Helper functions (feel free to add/remove/edit!)
//

// free list bins
// |~16|17~32|33~48|49~62|...|496~512|513~1024|1024~|
// | 0 |  1  |  2  |  3  |   |  31   |
int get_heap_idx(size_t *size){
  int heap_idx;
  if (*size <= 512) heap_idx = (*size-1)/16;
  else if (*size <= 1024) heap_idx = 32;
  else heap_idx = 33;
  return heap_idx;
}

void my_add_to_free_list(my_metadata_t *metadata) {
  // assert(!metadata->next); 
  my_heap_t *my_heap;
  int heap_idx = get_heap_idx(&metadata->size);
  my_heap = &my_heaps[heap_idx];

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

void merge(my_metadata_t *base_metadata) {
  // base_metadata の右隣と左隣に空き領域があるかを見る
  // 
  // ... | left_metadata | slot | base_metadata | object | right_metadata | slot | ...
  //     ^                      ^          ^             ^
  //     left_metadata          base_      ptr           right_metadata
  //     <---------------><-----><--------><------------><---------------><------>
  //          16 or 8                8       base_size        16 or 8      
  //

  int if_right = 0;
  int if_left = 0;
  int heap_idx;
  my_heap_t *my_heap;
  my_metadata_t *metadata;
  my_metadata_t *prev;
  my_metadata_t *right; // right of metadata
  my_metadata_t *right_metadata;
  my_metadata_t *right_prev = NULL;
  my_metadata_t *left_metadata = NULL;
  my_metadata_t *left_prev = NULL;

  // base_metadata の右隣
  right_metadata = (my_metadata_t *)((char *)base_metadata + HALF_META_SIZE + base_metadata->size);
  // free lists を全て見る
  for (heap_idx = 0; heap_idx < 34; heap_idx++) {
    my_heap = &my_heaps[heap_idx];
    metadata = my_heap->free_head;
    prev = NULL;
    while (metadata) {
      // metadata の右隣
      right = (my_metadata_t *)((char *)metadata + HALF_META_SIZE + metadata->size);
      // right_metadata のひとつ前が分かる
      if (metadata == right_metadata) {
        assert(if_right == 0);
        right_prev = prev;
        if_right = 1;
      }
      // metadata の右隣が base_metadata (= base_metadata の左隣が metadata)
      if (right == base_metadata) {
        assert(if_left == 0);
        left_metadata = metadata;
        left_prev = prev;
        if_left = 1;
      }
      prev = metadata;
      metadata = metadata->next;
    }
  }
  if (!if_right && !if_left) {
    // 右側にも左側にも空き領域がない
    my_add_to_free_list(base_metadata);
    return;
  }
  if (if_right && if_left) {
    // 両側に空き領域がある
    heap_idx = get_heap_idx(&right_metadata->size);
    my_remove_from_free_list(right_metadata, right_prev, &my_heaps[heap_idx]); 

    if(left_prev == right_metadata) left_prev = right_prev;
    heap_idx = get_heap_idx(&left_metadata->size);
    my_remove_from_free_list(left_metadata, left_prev, &my_heaps[heap_idx]);

    left_metadata->size += base_metadata->size + right_metadata->size + 2*HALF_META_SIZE;
    my_add_to_free_list(left_metadata);
    return;
  }
  if (if_right) {
    // 右側にだけ空き領域がある
    heap_idx = get_heap_idx(&right_metadata->size);
    my_remove_from_free_list(right_metadata, right_prev, &my_heaps[heap_idx]);

    base_metadata->size += right_metadata->size + sizeof(my_metadata_t);
    my_add_to_free_list(base_metadata);
    return;
  }
  if (if_left) {
    // 左側にだけ空き領域がある
    heap_idx = get_heap_idx(&left_metadata->size);
    my_remove_from_free_list(left_metadata, left_prev, &my_heaps[heap_idx]);

    left_metadata->size += base_metadata->size + sizeof(my_metadata_t);
    my_add_to_free_list(left_metadata);
    return;
  }
}

// for debug
void print_free_lists() {
  int heap_idx;
  my_heap_t *my_heap;
  my_metadata_t *metadata;
  for(heap_idx = 0; heap_idx < 34; heap_idx++) {
    my_heap = &my_heaps[heap_idx];
    metadata = my_heap->free_head;
    printf("---heap%d---\n", heap_idx);
    while (metadata) {
      printf("metadata: %p\n", metadata);
      printf("size: %ld\n", metadata->size);
      metadata = metadata->next;  
    }
  }
}

//
// Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
//

// This is called at the beginning of each challenge.
void my_initialize() {
  for (int i = 0; i < 34; i++) {
    my_heaps[i].dummy.size = 0;
    my_heaps[i].dummy.next = NULL;
    my_heaps[i].free_head = &my_heaps[i].dummy;
  }
}

// my_malloc() is called every time an object is allocated.
// |size| is guaranteed to be a multiple of 8 bytes and meets 8 <= |size| <=
// 4000. You are not allowed to use any library functions other than
// mmap_from_system() / munmap_to_system().
/* ほしいサイズを受け取って、そのサイズ分の領域を確保し、その領域の先頭アドレスを返す */
void *my_malloc(size_t size) {

  // size が大きいときは直接OSに要求し、freeするときも直接OSに返す
  if (size >= 3520) {
    size_t buffer_size = 4096;
    my_metadata_t *metadata = (my_metadata_t *)mmap_from_system(buffer_size);
    metadata->size = 4000;
    // free slot を結合してたまたま size が4000となった後に
    // 使用されたものと区別がつくようにありえない値にしておく
    metadata->next = (my_metadata_t *)-1;
    return (my_metadata_t *)metadata + 1;
  }
// printf("debug00\n");
  my_metadata_t *metadata;
  my_metadata_t *prev;
  my_metadata_t *best_fit = NULL;
  my_metadata_t *best_prev = NULL;

  // サイズによってどのビンを扱うか分ける
  my_heap_t *my_heap;
  int heap_idx = get_heap_idx(&size);

  for (; heap_idx < 34; heap_idx++) {
    my_heap = &my_heaps[heap_idx];

    // 空き領域の先頭
    metadata = my_heap->free_head;
    prev = NULL;
    // best_fit: Find the smallest free slot the object fits.

    while (metadata) {
      if (metadata->size >= size && 
        (!best_fit || (best_fit && metadata->size < best_fit->size))) {
        best_fit = metadata;
        best_prev = prev;
      }
      if (best_fit && best_fit->size == size) break;
      prev = metadata;
      metadata = metadata->next;  
    }
    // 後でheap_idx を使うため、33 になったら break している
    if (best_fit || heap_idx == 33) break;
  }
  // now, best_fit points to the best_fit free slot
  // metadata points to NULL
  // and best_prev is the previous entry of best_fit.

  if (!best_fit) {
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
    metadata->size = buffer_size - HALF_META_SIZE;
    metadata->next = NULL;
    // Add the memory region to the free list.
// printf("debug01\n");
    merge(metadata);
    // Now, try my_malloc() again. This should succeed.  
    return my_malloc(size);
  }

  // |ptr| is the beginning of the allocated object.
  // Put object in the last part of the free slot.
  // ... | metadata |   the free slot    | ...
  //           <------------------------->
  //                 metadata_size
  // ... | free slot | metadata | object | ...
  //     ^           ^          ^
  //     best_fit    new        ptr
  //    <----------->      <------------->
  //    remaining size            size
  // Remove the free slot from the free list.
  // metadata の部分は free list 情報で必要になるから取っておく
  size_t remaining_size = best_fit->size - size;
  void *ptr;
  my_metadata_t *new_metadata;

// printf("debug02\n");
  if (remaining_size > sizeof(my_metadata_t)) {
    // ... | free slot | metadata | free | ...
    // ... | free slot | met|   object   | ...
    //     ^           ^    ^
    //     best_fit    new  ptr
    //    <----------->     <------------>
    //    remaining size        size
    ptr = (my_metadata_t *)((char *)best_fit + remaining_size + HALF_META_SIZE);
    new_metadata = (my_metadata_t *)((char *)best_fit + remaining_size);
    new_metadata->size = size;
    new_metadata->next = NULL;

    my_remove_from_free_list(best_fit, best_prev, &my_heaps[heap_idx]);
    // Create a new metadata for the remaining free slot.
    //
    // ... | metadata | free slot | metadata | object | ...
    //     ^                      ^          ^
    //     best_fit               new        ptr
    //    <---------------------->           <-------->
    //        remaining size                   size
    best_fit->size = remaining_size - HALF_META_SIZE;
    // Add the remaining free slot to the free list.
    my_add_to_free_list(best_fit);
  } 
  else {
    ptr = (char *)best_fit + HALF_META_SIZE;
    my_remove_from_free_list(best_fit, best_prev, &my_heaps[heap_idx]);   
  }
// printf("debug03\n");
  
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

  my_metadata_t *metadata = (my_metadata_t *)((char *)ptr - sizeof(metadata));
  if (metadata->size >= 3520 && metadata->next == (my_metadata_t *)-1) {
    size_t buffer_size = 4096;
// printf("a:\n");
    munmap_to_system(metadata, buffer_size);
    return;
  }
  metadata = (my_metadata_t *)((char *)ptr - HALF_META_SIZE);
  // if there are free slots next to the ptr, 
  // merge them and add it to the free list
// printf("debug05\n");
  merge(metadata);
// printf("debug06\n");
  // my_add_to_free_list(metadata);
}

// This is called at the end of each challenge.
void my_finalize() {
  // Nothing is here for now.
  // feel free to add something if you want!
}

void test() {
  // Implement here!
  // void *ptr1 = my_malloc(100);
  // void *ptr2 = my_malloc(200);
  // void *ptr3 = my_malloc(300);
  // void *ptr4 = my_malloc(4000);
  // my_free(ptr2);
  // print_free_lists();
  // printf("\n");
  // my_free(ptr3);
  // print_free_lists();
  // printf("\n");
  // my_free(ptr4);
  // print_free_lists();
  // printf("\n");
  // exit(EXIT_SUCCESS);
}

