section .text
  global _start
    _start:
			BITS 32
			jmp short two
		one:
			pop ebx 					; [1]
			xor eax, eax
			push eax ; env[2]=0
			push eax ; terminates env[1] string
			push "a=11" ; env[1]
			mov esi, esp ; saves pointer to env[1]
			push eax ; terminates env[0] string
			push "b=22" ; env[0]
			mov edi, esp ; saves pointer to env[0]
			push eax
			; push pointers to env and save in edx beginning of env
			push edi
			push esi
			mov edx, esp
			
			mov al,  0x0b  
			int 0x80
		two:
			call one
			db '/usr/bin/env'
