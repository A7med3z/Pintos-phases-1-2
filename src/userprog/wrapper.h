#include <list.h>
#include "userprog/actualcall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "lib/syscall-nr.h"
#include "userprog/syscall.h"

void halt_wrapper (void);
void exit_wrapper (struct intr_frame *f);
tid_t exec_wrapper (struct intr_frame *f);
tid_t wait_wrapper (struct intr_frame *f);
bool create_wrapper (struct intr_frame *f);
bool remove_wrapper (struct intr_frame *f);
int open_wrapper (struct intr_frame *f);
int fileSize_wrapper (struct intr_frame *f);
int Read_wrapper (struct intr_frame *f);
int write_wrapper (struct intr_frame *f);
void seek_wrapper (struct intr_frame *f);
unsigned tell_wrapper (struct intr_frame *f);
void close_wrapper (struct intr_frame *f);
 