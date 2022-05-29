#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include<list.h>
#include "threads/synch.h"

void syscall_init (void);
int get_int (int** esp);
char* get_char_ptr (char*** esp);
void* get_void_ptr (void*** esp);
void validate_void_ptr (const void* pt);

struct file_opened{
   int fd;
   struct file* ptr ;
   struct list_elem elem ;
};

static struct lock files_sync_lock ;

#endif /* userprog/syscall.h */
