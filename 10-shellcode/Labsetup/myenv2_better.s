section .text
  global _start
    _start:
			BITS 32
			jmp short two
		one:
			pop ebx 					
			xor eax, eax
			mov [ebx+12], al ; replace placeholder * with zero
			;push eax ; env[2]=0
			push eax ; terminate env[1]
			push dword [ebx+13] ; push env[1]
			push eax ; terminate env[0]
			push dword [ebx+17] ; push env[0]
			
			push eax
			lea esi, [esp+4]
			push esi
			lea esi, [esp+16]
			push esi
			mov edx, esp
			
			mov al,  0x0b     ; usual stuff
			int 0x80
		two:
			call one
			db '/usr/bin/env*a=11b=22'
