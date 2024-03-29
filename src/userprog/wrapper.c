#include <list.h>
#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/wrapper.h"
#include "userprog/syscall.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "syscall-nr.h"
#include "filesys/file.h"
#include "lib/kernel/stdio.h"
#include "threads/synch.h"
#include "filesys/filesys.h"
#include "threads/vaddr.h"
#include "filesys/off_t.h"
#include "threads/malloc.h"
#include "threads/vaddr.h"
#include "userprog/process.h"

void release_children(struct thread* t);
void close_files(struct thread* t);
void remove_parent_list(struct thread* t);
struct file_opened* get_file_d(int fd);

void
halt (void)
{
    shutdown_power_off ();
}

void
exit (int status)
{
    struct thread * t = thread_current();
    if (!list_empty (&t->open_file));
        close_files (t);
    if (t->executable_file != NULL)
        file_allow_write (t->executable_file);
    if (t->parent_thread->waiting_on == t->tid){
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

tid_t
exec (const char *cmd_line)
{
    return process_execute (cmd_line);
}

tid_t
wait (int pid)
{
    return process_wait (pid);
}

bool
create (const char *file, unsigned initial_size)
{
    if (file == NULL)
	    exit (-1);
    lock_acquire (&files_sync_lock);
    bool created = filesys_create (file, initial_size);
    lock_release (&files_sync_lock);
    return created;
}

bool
rmv (const char *file)
{
    lock_acquire (&files_sync_lock);
    bool removed = filesys_remove (file);
    lock_release (&files_sync_lock);
    return removed;
}

int open (const char *file){
    if(file == NULL)
	return -1;
    lock_acquire (&files_sync_lock);
    struct file* trial = filesys_open (file);
    if (trial == NULL)
        return -1;
    struct file_opened *new_file = malloc (sizeof (struct file_opened));
    new_file->fd = thread_current ()->fd_last++;
    lock_release (&files_sync_lock);
    new_file->ptr = trial;
    list_push_back (&thread_current ()->open_file, &new_file->elem);
    return new_file->fd;
}

int
filesize (int fd)
{
    struct file_opened* file_d = get_file_d (fd);
    if (file_d == NULL || file_d->ptr == NULL)
        return -1;
    lock_acquire (&files_sync_lock);
    int size = file_length (file_d ->ptr);
    lock_release (&files_sync_lock);
    return size;
}

int
read (int fd, void *buffer, unsigned size)
{
    if (fd == 0)
        return input_getc ();
    struct file_opened* file_d = get_file_d (fd);
    if (file_d == NULL || file_d->ptr == NULL || buffer == NULL)
        return -1;
    lock_acquire (&files_sync_lock);
    unsigned bytes_read = file_read (file_d->ptr, buffer,size);
    lock_release (&files_sync_lock);
    return bytes_read;
}

int
write (int fd, const void *buffer, unsigned size)
{
    if (fd == 1){
       putbuf ((char*) buffer, size);
       return size;
    }
    struct file_opened* file_d = get_file_d (fd);
    if (file_d == NULL || file_d->ptr == NULL || buffer == NULL)
        return -1;
    lock_acquire (&files_sync_lock);
    unsigned bytes_written = file_write (file_d ->ptr, buffer, size);
    lock_release (&files_sync_lock);
    return bytes_written;
}

void
seek (int fd, unsigned position)
{
    struct file_opened* file_d = get_file_d (fd);
    if (file_d == NULL || file_d->ptr == NULL)
        return;
    unsigned curr_pos = tell (fd);
    lock_acquire (&files_sync_lock);
    file_seek (file_d->ptr, position);
    lock_release (&files_sync_lock);
}

unsigned
tell (int fd)
{
    struct file_opened* file_d = get_file_d (fd);
    if (file_d == NULL || file_d->ptr == NULL)
        return -1;
    lock_acquire (&files_sync_lock);
    unsigned val = file_tell (file_d ->ptr);
    lock_release (&files_sync_lock);
    return val;
}

void
close (int fd)
{
    struct file_opened *file_d = get_file_d (fd);
    if (file_d == NULL)
        return;
    lock_acquire (&files_sync_lock);
    file_close (file_d->ptr);
    list_remove (&file_d->elem);
    lock_release (&files_sync_lock);
    free (file_d);
}

void
close_files(struct thread *t)
{
    struct list_elem *e;
    while (!list_empty (&t->open_file))
    {
        e = list_pop_front (&t->open_file);
        struct file_opened *file_d = list_entry (e, struct file_opened, elem);
     	file_close (file_d->ptr);
     	list_remove (&file_d->elem);
     	free (file_d);
    }
}

void
release_children (struct thread *t)
{
    struct list_elem* e;
    while (!list_empty (&t->child_list))
    {
        e = list_pop_front (&t->child_list);
        struct child_process *child = list_entry (e, struct child_process, elem);
        list_remove (&child->elem);
        sema_up (&child->t->parent_child_sync);
        free (child);
    }
}

void
remove_parent_list (struct thread* t)
{
    struct list_elem* e;
    for (e = list_begin (&t->parent_thread->child_list); e != list_end (&t->parent_thread->child_list); e = list_next (e)){
        struct child_process *child = list_entry (e, struct child_process, elem);
        if (child ->pid == t->tid)
        {
            list_remove (&child->elem);
            free (child);
            break;
        }
    }
}

struct file_opened*
get_file_d (int fd)
{
    struct thread *t = thread_current ();
    struct file_opened *file_d = NULL;
    struct list_elem* e;
    lock_acquire (&files_sync_lock);
    if (!list_empty (&t->open_file))
    {
        for (e = list_begin (&t->open_file); e != list_end (&t->open_file); e = list_next (e)){
            struct file_opened *temp = list_entry (e, struct file_opened, elem);
            if (temp->fd == fd)
            {
                file_d = temp;
                break;
            }
        }
    }
    lock_release (&files_sync_lock);
    return file_d;
}


void
wrapper_exit (struct intr_frame *f)
{
	int status = get_int ((int**) f->esp, 1);
	validate_void_ptr ((const void*) status);
	exit (status);
}

tid_t
wrapper_exec (struct intr_frame *f)
{
	char* cmd_line = get_char_ptr ((char***) (f->esp), 1);
	validate_void_ptr ((const void*) cmd_line);
 	return exec (cmd_line);
}

tid_t
wrapper_wait (struct intr_frame *f)
{
	int pid = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) pid);
	return wait (pid);
}

bool
wrapper_create (struct intr_frame *f)
{
	char* file = get_char_ptr ((char***) (f->esp), 1);
	validate_void_ptr ((const void*) file);
	int initial_size = get_int ((int**) (f->esp), 2);
	validate_void_ptr ((const void*) initial_size);
	return create (file, initial_size);
}


bool
wrapper_remove (struct intr_frame *f)
{
	char* file = get_char_ptr ((char***) (f->esp), 1);
	validate_void_ptr ((const void*) file);
	return rmv (file);
}


int
wrapper_open (struct intr_frame *f)
{
	char* file = get_char_ptr ((char***) (f->esp), 1);
	validate_void_ptr ((const void*) file);
	return open (file);
}


int
wrapper_filesize (struct intr_frame *f)
{
	int fd = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) fd);
	return filesize (fd);
}

int
wrapper_read (struct intr_frame *f)
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
wrapper_write (struct intr_frame *f){
	int fd = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) fd);
	void *buffer = get_void_ptr ((void***) (f->esp), 2);
	validate_void_ptr ((const void*) buffer);
	unsigned size = get_int ((int**) (f->esp), 3);
	validate_void_ptr ((const void*) size);
	return write (fd, buffer, size);
}

void
wrapper_seek (struct intr_frame *f)
{
	int fd = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) fd);
	unsigned position = get_int ((int**) (f->esp), 2);
	validate_void_ptr ((const void*) position);
	seek (fd, position);
}

unsigned
wrapper_tell (struct intr_frame *f)
{
	int fd = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) fd);
	return tell (fd);
}

void
wrapper_close (struct intr_frame *f)
{
	int fd = get_int ((int**) (f->esp), 1);
	validate_void_ptr ((const void*) fd);
	close (fd);
}