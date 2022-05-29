#include <list.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/vaddr.h"
#include "filesys/off_t.h"
#include "userprog/wrapper.h"
#include "userprog/process.h"


void
seek (int fd, unsigned position)
{  
    struct file *file = get_file (fd);
    if (file == NULL)
    {
        return ;
    }
    lock_acquire (&files_sync_lock);
    file_seek (file,position);
    lock_release (&files_sync_lock);
}

unsigned
tell (int fd)
{
    struct file *file = get_file (fd);
    if (file == NULL)
    {
        return ;
    }
    lock_acquire (&files_sync_lock);
    file_tell (file);
    lock_release (&files_sync_lock);
}

void
close (int fd)
{
  struct file_opened *file = get_file_opened (fd);
  if (file == NULL && file->ptr == NULL)
  {
      return ;
  }
  lock_acquire (&files_sync_lock);
  file_close (file->ptr);
  list_remove (&file->elem);
  lock_release (&files_sync_lock);  
  free (file);
}

void
halt_wrapper ()
{
    shutdown_power_off();
}

int
exit_wrapper (struct intr_frame *f)
{
    int status = get_int ((int**) f->esp, 1);
	validate_void_ptr ((const void*) status);
	exit (status);
}

tid_t
exec_wrapper (struct intr_frame *f)
{
    char* cmd_line = get_char_ptr ((char***) (f->esp), 1);
	validate_void_ptr ((const void*) cmd_line);
    return process_execute (cmd_line);
}

int
wait_wrapper (struct intr_frame *f)
{
    int pid = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) pid);
	return process_wait (pid);
}

bool
create_wrapper (struct intr_frame *f)
{
    char* file = get_char_ptr((char***) (f->esp), 1);
	validate_void_ptr ((const void*) file);
	int initial_size = get_int ((int**) (f->esp), 2);
	validate_void_ptr ((const void*) initial_size);
    if (file == NULL)
	    exit (-1);
    lock_acquire (&sys_lock);
    bool created = filesys_create (file, initial_size);
    lock_release (&sys_lock);
    return created;
}

bool
remove_wrapper (struct intr_frame *f)
{
    char* file = get_char_ptr ((char***) (f->esp), 1);
	validate_void_ptr ((const void*) file);
    lock_acquire (&sys_lock);
    bool removed = filesys_remove (file);
    lock_release (&sys_lock);
    return removed;
}

int
open_wrapper (struct intr_frame *f)
{
    char* file = get_char_ptr ((char***) (f->esp), 1);
	validate_void_ptr ((const void*) file);
	return open (file);
}

int
fileSize_wrapper (struct intr_frame *f)
{
    int fd = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) fd);
	return filesize (fd);
}

int
Read_wrapper (struct intr_frame *f)
{
    int fd = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) fd);
	void *buffer = get_void_ptr ((void***) (f->esp), 2);
	validate_void_ptr ((const void*) buffer);
	unsigned size = get_int ((int**) (f->esp), 3);
	validate_void_ptr ((const void*) size);
	return read (fd, buffer, size);
}

int
write_wrapper (struct intr_frame *f)
{
    //not impelemented
}

void
seek_wrapper (struct intr_frame *f)
{
    int fd = get_int ((int**) f) + 1;
    validate_void_ptr ((const void*) fd);
    unsigned position = get_int ((int**) f) + 2;
    validate_void_ptr ((const void*) position);
    seek (fd, position);
}

unsigned
tell_wrapper (struct intr_frame *f)
{
    int fd = get_int ((int**) f) + 1;
    validate_void_ptr ((const void*) fd);
    return tell (fd);
}

void
close_wrapper (struct intr_frame *f)
{
    int fd = get_int ((int**) f) + 1;
    validate_void_ptr ((const void*)fd);
    close (fd);
}



struct file*
get_file (int fd)
{
    struct file_opened *file = get_file_opened (fd);
    if (file != NULL)
        return file->ptr ;
    return NULL;
}

struct file_opend*
get_file_opened (int fd)
{
    struct thread *t = thread_current ();
    struct list_elem *e;
    for (e = list_begin (&t->open_file); e != list_end (&t->open_file);
         e = list_next (e)) 
    {
        struct file_opened *tempf = list_entry (e, struct thread, elem);
        if (tempf == NULL)
            return NULL;
        else if (tempf->fd == fd)
        {
        return tempf;
        }
    }
  return NULL;
}

void
exit (int status) {
    struct thread * t = thread_current();
    if (!list_empty (&t->files))
        close_files (t);
    if (t->fd_exec != NULL)
        file_allow_write (t->fd_exec);
    if (t->parent_thread->waiting_on == t->tid)
    {
        t->parent_thread->child_status = status;
        t->parent_thread->waiting_on = -1;
        sema_up (&t->parent_thread->parent_child_sync);
    }
    else
        remove_parent_list (t);
    if (!list_empty (&t->child_list))
        release_children (t);
    thread_exit ();
}

int
open (const char *file)
{
    if (file == NULL)
	    return -1;
    lock_acquire (&sys_lock);
    struct file* trial = filesys_open (file);
    if (trial == NULL)
        return -1;
    struct file_descriptor *new_file = malloc (sizeof (struct file_descriptor));
    new_file->fd = thread_current()->fd_last++;
    lock_release (&sys_lock);
    new_file->file = trial;
    list_push_back (&thread_current ()->files, &new_file->elem);
    return new_file->fd;
}