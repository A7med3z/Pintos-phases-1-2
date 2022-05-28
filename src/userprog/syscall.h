#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
int get_int(int** esp);
char* get_char_ptr(char*** esp);
void* get_void_ptr(void*** esp);
void validate_void_ptr(const void* pt);

#endif /* userprog/syscall.h */