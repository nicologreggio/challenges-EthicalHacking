; Allocate space for the command string and the arguments
section .data
command db '/bin/ls', 0 ; The command to execute
arg1 db '-l', 0 ; The first argument
arg2 db '-a', 0 ; The second argument

section .text
global _start
_start:
    ; Push the arguments onto the stack in reverse order
    push dword 0 ; Push a null pointer onto the stack
    push dword offset arg2 ; Push the address of arg2 onto the stack
    push dword offset arg1 ; Push the address of arg1 onto the stack
    push dword offset command ; Push the address of command onto the stack

    ; Set the values of the registers required by the execve system call
    mov eax, 11 ; Set the execve system call number
    mov ebx, esp ; Set the stack pointer as the first argument
    xor ecx, ecx ; Set the second argument to null
    xor edx, edx ; Set the third argument to null

    ; Call the execve system call
    int 0x80