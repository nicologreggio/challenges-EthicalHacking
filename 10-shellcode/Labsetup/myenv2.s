section .text
  global _start
    _start:
			BITS 32
			jmp short two
		one:
			pop ebx 					; [1]
			xor eax, eax
			push eax
			push eax
			push "a=11"
			mov esi, esp
			push eax
			push esi
			mov edx, esp
			;mov [ebx+4], al
			;mov [ebx+17], al
			;mov [ebx+22], ax
			;mov [ebx+7], al		; save 0x00 (1 byte) to memory at address ebx+7
			;mov [ebx+8], eax 	; save ebx (4 bytes) to memory at address ebx+8
			;mov [ebx+12], eax	; save eax (4 bytes) to memory at address ebx+12
			;lea ecx, [ebx+8] 	; let ecx = ebx + 8
			;xor ecx, ecx
			;lea edx, [ebx+13]
			
			mov al,  0x0b     ; usual stuff
			int 0x80
		two:
			call one
			; /usr/bin/env a=11 b=22 
			;db '/usr/bin/env*a=11*b=22*'  ; [2]
			db '/usr/bin/env'
			;db '/usr/bin/env*a=11**'
			;db '/bin/sh*AAAABBBB'  ; [2]
