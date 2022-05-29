#ifndef USERPROG_WRAPPER_H
#define USERPROG_WRAPPER_H

#include <list.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"

void halt_wrapper ();
int exit_wrapper (struct intr_frame *f);
tid_t exec_wrapper (struct intr_frame *f);
int  wait_wrapper (struct intr_frame *f);
bool create_wrapper (struct intr_frame *f);
bool remove_wrapper (struct intr_frame *f);
int open_wrapper (struct intr_frame *f);
int fileSize_wrapper (struct intr_frame *f);
int Read_wrapper (struct intr_frame *f);
int write_wrapper (struct intr_frame *f);
void  seek_wrapper (struct intr_frame *f);
unsigned tell_wrapper (struct intr_frame *f);
void close_wrapper (struct intr_frame *f);


void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);

struct file* get_file (int fd);
struct file_opend* get_file_opened (int fd);
 

#endif
