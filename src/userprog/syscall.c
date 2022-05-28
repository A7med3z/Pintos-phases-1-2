#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <list.h> 
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/wrapper.h"

static void syscall_handler (struct intr_frame *);
static struct lock files_sync_lock ;

struct file_opened{
   int fd;
   struct file* ptr ;
   struct list_elem elem ;
};

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

int 
get_int (int** esp)
{
  int sys_code = *(int*)esp ;
  return sys_code;
}

char*
get_char_ptr (char*** esp){
  char* sys_code = (char*)(*(int*)esp);
  return sys_code;
}

void* 
get_char_ptr (void*** esp)
{
  void* sys_code = (void*)(*(int*)esp);
  return sys_code;
}

void 
validate_void_ptr (const void* pt)
{
     if (!is_user_vaddr (pt))
     {
       exit (-1);
     }
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  void** esp = f->esp;
  validate_void_ptr (esp);

  switch (get_int (esp))
   {
     case SYS_HALT:{
       halt_wrapper ();
       break;
     }
     case SYS_EXIT:{
       exit_wrapper (f);
       break;
     }
     case SYS_EXEC:{
       f->eax = exec_wrapper (f);
       break;
     }
     case SYS_WAIT:{
       f->eax = wait_wrapper ();
       break;
     }
     case SYS_CREATE:{
       f->eax = create_wrapper (f);
       break;
     }
     case SYS_REMOVE:{
       f->eax = remove_wrapper (f);
       break;
     }
     case SYS_OPEN:{
       f->eax = open_wrapper (f);
       break;
     }
     case SYS_FILESIZE:{
       f->eax = fileSize_wrapper (f);
       break;
     }
     case SYS_READ:{
       f->eax = Read_wrapper ();
       break;
     }
     case SYS_WRITE:{
       f->eax = write_wrapper (f);
       break;
     }
     case SYS_SEEK:{
       seek_wrapper (f);
       break;
     }
     case SYS_TELL:{
       f->eax = tell_wrapper (f);
       break;
     }
     case SYS_CLOSE:{
       close_wrapper (f);
       break;
     }
   }
  printf ("system call!\n");
  thread_exit ();
}