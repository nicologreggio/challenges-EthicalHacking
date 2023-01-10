section .text
  global _start
    _start:
      ; Store the argument string on stack
      xor  eax, eax 
      push eax          ; Use 0 to terminate the string
      push "/env"
      push "/bin"       ; [1]
      push "/usr"
      mov  ebx, esp     ; Get the string address

      ; env[3] = 0          // 0 marks the end of the array
      ;push eax          

      ; env[2] = address to the "cccc=1234" string
      ;push eax
      mov edi, "4###"
      shl edi, 24
      shr edi, 24
      push edi
      push "=123"
      push "cccc"
      mov edi, esp

      ; env[1] = address to the "bbb=5678" string
      push eax
      push "5678"
      push "bbb="
      mov esi, esp

      ; env[0] = address to the "aaa=1234" string
      push eax
      push "1234"
      push "aaa="
      mov edx, esp
      
      push eax
      push ebx          ; argv[0] points "/usr/bin/env"   [3]
      mov  ecx, esp     ; Get the address of argv[]
   
      ; For environment variable 
      push eax
      push edi
      push esi
      push edx

      mov edx, esp

      ; Invoke execve()
      xor  eax, eax     ; eax = 0x00000000
      mov   al, 0x0b    ; eax = 0x0000000b
      int 0x80
