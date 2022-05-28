#include <stdio.h>
#include <list.h>
#include "userprog/wrapper.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "lib/syscall-nr.h"
#include "lib/kernel/stdio.h"
#include "userprog/syscall.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "filesys/off_t.h"


void
halt_wrapper ()
{
    shutdown_power_off ();
}

void
exit_wrapper (struct intr_frame *f)
{
    
}

pid_t
exec_wrapper (struct intr_frame *f)
{
    return 
}

int
wait_wrapper (struct intr_frame *f)
{

}

bool
create_wrapper (struct intr_frame *f)
{

}

bool
remove_wrapper (struct intr_frame *f)
{

}

int
open_wrapper (struct intr_frame *f)
{

}

int
fileSize_wrapper (struct intr_frame *f)
{

}

int
Read_wrapper (struct intr_frame *f)
{

}

int
write_wrapper (struct intr_frame *f)
{

}

void
seek_wrapper (struct intr_frame *f)
{

}

unsigned
tell_wrapper (struct intr_frame *f)
{

}

void
close_wrapper (struct intr_frame *f)
{

}