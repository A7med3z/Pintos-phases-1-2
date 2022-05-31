#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <list.h>
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "lib/syscall-nr.h"

void syscall_init (void);

struct file_opened{
   int fd;
   struct file* ptr ;
   struct list_elem elem ;
};

struct lock files_sync_lock ;

#endif /* userprog/syscall.h */
